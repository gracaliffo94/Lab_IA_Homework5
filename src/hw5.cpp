#include <ros/ros.h>
#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/filters/passthrough.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/visualization/point_cloud_color_handlers.h>
#include <sensor_msgs/PointCloud2.h>

class Hw5 {
protected:
	ros::NodeHandle nh;
	ros::Subscriber pcl_sub;
public:
	Hw5(){
		pcl_sub = nh.subscribe<sensor_msgs::PointCloud2>("camera/depth/points", 1000, &Hw5::pclCallback,this);
		ROS_INFO("Subscriber OK");
	}

	void pclCallback(sensor_msgs::PointCloud2 pcl_msg){
		pcl::PointCloud<pcl::PointXYZ>::Ptr pcl(new pcl::PointCloud<pcl::PointXYZ>);
		pcl::PassThrough<pcl::PointXYZ> p_th;
		pcl::fromROSMsg(pcl_msg, *pcl);
		p_th.setInputCloud (pcl);
		p_th.setFilterLimits (0.5, 10.0);
		p_th.setFilterFieldName ("z");
		p_th.filter( *pcl);
		pcl::VoxelGrid<pcl::PointXYZ>  v_gr;
		v_gr.setInputCloud (pcl);
		v_gr.setLeafSize (0.01, 0.01, 0.01);
		v_gr.filter ( *pcl ) ;
		boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer (new pcl::visualization::PCLVisualizer ("PCL_V"));
		viewer->addPointCloud<pcl::PointXYZ> (pcl, "Cloud" );
		viewer->spin(); 
           }
};
		
int main (int argc, char** argv) {

  ros::init(argc, argv, "hw5");

  Hw5 handler;
  ros::spin();

  return (0);
}
