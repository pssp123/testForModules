#include <iostream>
#include <opencv2/core/core.hpp>
#include <ceres/ceres.h>
#include <chrono>

using namespace std;

 // 代价函数的计算模型
 struct CURVE_FITTING_COST
 {
 CURVE_FITTING_COST ( double x, double y):ax ( x ), ay ( y ) {}
 // 残差的计算
 template <typename T>
 bool operator() (
 const T* const abc, // 模型参数，有 3 维
 T* residual ) const // 残差
 {
 // y-exp(ax^2+bx+c)
 residual[0] = T ( ay ) - ceres::exp ( abc[0]*T ( ax ) *T ( ax ) + abc[1]*T ( ax ) + abc[2] );
 return true;
 }
 
 const double ax,ay; // x,y 数据
 };

 int main ( int argc, char** argv )
 {
 double a=1.0, b=2.0, c=1.0; // 真实参数值
 int N=100; // 数据点
 double w_sigma=1.0; // 噪声 Sigma 值
 cv::RNG rng; // OpenCV 随机数产生器
 double abc[3] = {0,0,0}; // abc 参数的估计值

 vector<double> xdata, ydata; // 数据

 cout<<"generating data: "<<endl;
 for ( int i=0; i<N; i++ )
 {
 double x = i/100.0;
 xdata.push_back ( x );
 ydata.push_back (
 exp ( a*x*x + b*x + c ) + rng.gaussian ( w_sigma )
 );
 printf("%f ",ydata[i]);
 //cout<<xdata[i]<<""<<ydata[i]<<endl;
 }

 // 构建最小二乘问题
 ceres::Problem problem;
 for ( int i=0; i<N; i++ )
 {
 problem.AddResidualBlock ( // 向问题中添加误差项
 // 使用自动求导，模板参数：误差类型，输出维度，输入维度，数值参照前面 struct 中写法
 new ceres::AutoDiffCostFunction<CURVE_FITTING_COST, 1, 3> (
 new CURVE_FITTING_COST ( xdata[i], ydata[i] )
 ),
 nullptr, // 核函数，这里不使用，为空
 abc // 待估计参数
 );
 }

 // 配置求解器
 ceres::Solver::Options options; // 这里有很多配置项可以填
 options.linear_solver_type = ceres::DENSE_QR; // 增量方程如何求解
 options.minimizer_progress_to_stdout = true; // 输出到cout

 ceres::Solver::Summary summary; // 优化信息
 chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
 ceres::Solve ( options, &problem, &summary ); // 开始优化
 chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
 chrono::duration<double> time_used = chrono::duration_cast<chrono::duration<double>>( t2-t1 );
 cout<<"solve time cost = "<<time_used.count()<<" seconds. "<<endl;

 // 输出结果
 cout<<summary.BriefReport() <<endl;
 cout<<"estimated a,b,c = ";
 for ( auto a:abc ) cout<<a<<" ";
 
 cout<<endl;

 return 0;
 }