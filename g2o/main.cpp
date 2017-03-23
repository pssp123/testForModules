 #include <iostream>
 #include <g2o/core/base_vertex.h>
 #include <g2o/core/base_unary_edge.h>
 #include <g2o/core/block_solver.h>
 #include <g2o/core/optimization_algorithm_levenberg.h>
 #include <g2o/core/optimization_algorithm_gauss_newton.h>
 #include <g2o/core/optimization_algorithm_dogleg.h>
 #include <g2o/solvers/dense/linear_solver_dense.h>
 #include <eigen3/Eigen/Core>
 #include <opencv2/core/core.hpp>
 #include <cmath>
 #include <chrono>
 using namespace std;

 
 class CurveFittingVertex: public g2o::BaseVertex<3, Eigen::Vector3d>
 {
 public:
 EIGEN_MAKE_ALIGNED_OPERATOR_NEW
 virtual void setToOriginImpl() // reset
 {
 _estimate << 0,0,0;
 }

 virtual void oplusImpl( const double* update ) // update
 {
 _estimate += Eigen::Vector3d(update);
 }
 // ŽæÅÌºÍ¶ÁÅÌ£ºÁô¿Õ
 virtual bool read( istream& in ) {}
 virtual bool write( ostream& out ) const {}
 };

 // Îó²îÄ£ÐÍ Ä£°å²ÎÊý£º¹Û²âÖµÎ¬¶È£¬ÀàÐÍ£¬Á¬œÓ¶¥µãÀàÐÍ
 class CurveFittingEdge: public g2o::BaseUnaryEdge<1,double,CurveFittingVertex>
 {

 public:
 EIGEN_MAKE_ALIGNED_OPERATOR_NEW
 CurveFittingEdge( double x ): BaseUnaryEdge(), _x(x) {}
 // ŒÆËãÇúÏßÄ£ÐÍÎó²î
 void computeError()
 {
 const CurveFittingVertex* v = static_cast<const CurveFittingVertex*> (_vertices[0]);
 const Eigen::Vector3d abc = v->estimate();
 _error(0,0) = _measurement - std::exp( abc(0,0)*_x*_x + abc(1,0)*_x + abc(2,0) ) ;
 }
 virtual bool read( istream& in ) {}
 virtual bool write( ostream& out ) const {}
 public:
 double _x; // x Öµ£¬ y ÖµÎª _measurement
 };

 int main( int argc, char** argv )
 {
 double a=1.0, b=2.0, c=1.0; // ÕæÊµ²ÎÊýÖµ
 int N=100; // ÊýŸÝµã
 double w_sigma=1.0; // ÔëÉùSigmaÖµ
 cv::RNG rng; // OpenCVËæ»úÊý²úÉúÆ÷
 double abc[3] = {0,0,0}; // abc²ÎÊýµÄ¹ÀŒÆÖµ

 vector<double> x_data, y_data; // ÊýŸÝ

 cout<<"generating data: "<<endl;
 for ( int i=0; i<N; i++ )
 {
 double x = i/100.0;
 x_data.push_back ( x );
 y_data.push_back (
 exp ( a*x*x + b*x + c ) + rng.gaussian ( w_sigma )
 );
 cout<<x_data[i]<<" "<<y_data[i]<<endl;
 }

 // ¹¹œšÍŒÓÅ»¯£¬ÏÈÉè¶šg2o
 // ŸØÕó¿é£ºÃ¿žöÎó²îÏîÓÅ»¯±äÁ¿Î¬¶ÈÎª 3 £¬Îó²îÖµÎ¬¶ÈÎª 1
 typedef g2o::BlockSolver< g2o::BlockSolverTraits<3,1> > Block;
 // ÏßÐÔ·œ³ÌÇóœâÆ÷£º³íÃÜµÄÔöÁ¿·œ³Ì
 Block::LinearSolverType* linearSolver = new g2o::LinearSolverDense<Block::PoseMatrixType>();
 Block* solver_ptr = new Block( linearSolver ); // ŸØÕó¿éÇóœâÆ÷
 // ÌÝ¶ÈÏÂœµ·œ·š£¬ŽÓGN, LM, DogLeg ÖÐÑ¡
 //g2o::OptimizationAlgorithmLevenberg* solver = new g2o::OptimizationAlgorithmLevenberg( solver_ptr );
 // È¡ÏûÏÂÃæµÄ×¢ÊÍÒÔÊ¹ÓÃ GN »ò DogLeg
 // g2o::OptimizationAlgorithmGaussNewton* solver = new g2o::OptimizationAlgorithmGaussNewton(solver_ptr );
  g2o::OptimizationAlgorithmDogleg* solver = new g2o::OptimizationAlgorithmDogleg( solver_ptr );

 g2o::SparseOptimizer optimizer; // ÍŒÄ£ÐÍ
 optimizer.setAlgorithm( solver ); // ÉèÖÃÇóœâÆ÷
 optimizer.setVerbose( true ); // Žò¿ªµ÷ÊÔÊä³ö

 // ÍùÍŒÖÐÔöŒÓ¶¥µã
 CurveFittingVertex* v = new CurveFittingVertex();
 v->setEstimate( Eigen::Vector3d(0,0,0) );
 v->setId(0);
 optimizer.addVertex( v );

 // ÍùÍŒÖÐÔöŒÓ±ß
 for ( int i=0; i<N; i++ )
 {
 CurveFittingEdge* edge = new CurveFittingEdge( x_data[i] );
 edge->setId(i);
 edge->setVertex( 0, v ); // ÉèÖÃÁ¬œÓµÄ¶¥µã
 edge->setMeasurement( y_data[i] ); // ¹Û²âÊýÖµ
 // ÐÅÏ¢ŸØÕó£ºÐ­·œ²îŸØÕóÖ®Äæ
 edge->setInformation( Eigen::Matrix<double,1,1>::Identity()*1/(w_sigma*w_sigma) );
 optimizer.addEdge( edge );
 }

 // ÖŽÐÐÓÅ»¯
 cout<<"start optimization"<<endl;
 chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
 optimizer.initializeOptimization();
 optimizer.optimize(100);
 chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
 chrono::duration<double> time_used = chrono::duration_cast<chrono::duration<double>>( t2-t1 );
 cout<<"solve time cost = "<<time_used.count()<<" seconds. "<<endl;

 // Êä³öÓÅ»¯Öµ
 Eigen::Vector3d abc_estimate = v->estimate();
 cout<<"estimated model: "<<abc_estimate.transpose()<<endl;

 return 0; }