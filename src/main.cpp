#include <renderer.h>
#include <goal_path_calculator.h>
#include <path_calculator.h>
#include <opencv2/opencv.hpp>
#include <socket.h>

void test_graphical();

int main () {
	Renderer::init();
	const double pi = std::acos(-1);
	//SocketServer sock(5070);

	while (true) {
		for (float i = 0; i < (pi * 2.0); i+= pi/10.0) {
			Path path = GoalPathCalculator::calculate_path(
					-i,  					//direction start
					cv::Point2f(0.0, 0.0),  //position start
					i,                      //direction end
					cv::Point2f(0.5, 0.5),  //position end
					0.5,                    //wheel distance
					1.0,                    //max allowed velocity
					0.01,                   //max time step
					0.05);                  //min velocity
			Path::TalonPoint next;
			while(path.next_point(&next)) {
				Renderer::clear();

				Renderer::bound(Renderer::get_spline_size(&path.spline), 4.0);

				Renderer::grid(1.0, 1.0, 0.2, 0.2, 0.2, Renderer::get_spline_size(&path.spline));

				path.render();

				Renderer::render_spline(&path.spline);

				float robot_angle = 0.0;
				cv::Point2f robot_pos (0.0, 0.0);
				path.get_current_loc_nondestruct(&robot_pos, &robot_angle);
				float min = std::min(next.velocity_left, next.velocity_right);
				Renderer::draw_robot(robot_angle, robot_pos, 1.5, 1.0, -min, min, 0.0);

				Renderer::display();
				//std::cout << path.official_traversal.spline_index << std::endl;
				//
			}

			/*
				do {
				std::cout << "Writing" << std::endl;
				sock.write_to(&next, sizeof(Path::TalonPoint));
				bool abort;
				std::cout << "Reading" << std::endl;
				sock.read_to(&abort, sizeof(bool)); //Read once before we update the spline
				if (abort) break;	
				} while(path.next_point(&next));
				*/
		}
	}
}
