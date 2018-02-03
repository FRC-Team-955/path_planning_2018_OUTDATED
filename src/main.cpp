#include <field_dimensions.h>
#include <field_renderer.h>
#include <multi_waypoint_calculator.h>
#include <opencv2/opencv.hpp>
#include <renderer.h>
#include <socket.h>

#define JUST_RENDER true

namespace FD = FieldDimension;
namespace MM = MiscMath;

float random_float() { return ((double)rand() / (RAND_MAX)); }

// TODO:
//    Make creating waypoints cleaner/easier
//    Documentation
//    Proper error reporting and logging; Return enum with error code instead of bool? 
//    	0 for success, any for fail for partial backwards compat.

int main() {
#if JUST_RENDER
	Renderer::init();
#else
	SocketServer sock(5801);
#endif

	srand(time(NULL));
	const float wheel_width = 660.0;

	MultiWaypointCalculator path(wheel_width / 2.0, 20.0);

	path.reset_and_begin(
			{FD::Switch::front_center_left - cv::Point2f(0.0, wheel_width / 2.0f),
			0.25, 1.0, MM::pi / 2.0f, wheel_width});
	for (int i = 0; i < 3; i++)
		path.push_back(
				{cv::Point2f(1000.0 + random_float() * (FD::field_width - 1000.0),
						1000.0 + random_float() * (FD::field_height - 2000.0)),
				1.0, 1.0, random_float() * MM::pi * 2.0f, wheel_width});

	size_t randspot = random_float() * 200.0;
	size_t idx = 0;

	TankDriveCalculator::TankOutput output;

	while (path.evaluate(output)) {
#if JUST_RENDER
		Renderer::clear();
		Renderer::bound(FD::field_bounds, 4.0);
		Renderer::grid(1000.0, 1000.0, 0.2, 0.2, 0.2, FD::field_bounds);
		FieldRenderer::render((char *)"RL", false);

		if (idx == randspot) {
			path.replace_current(
					{output.center_position +
					cv::Point2f(random_float(), random_float()) * 10.0,
					1.0, 1.0, output.robot_direction + (random_float() * 0.001f),
					wheel_width});
			randspot = (random_float() * 500.0) + idx;
		}

		path.render();
		Renderer::display();
		idx++;
#else
		bool abort;
		sock.write_to(&output.motion, sizeof(TankDriveCalculator::TankOutput));
		sock.read_to(&abort,
				sizeof(bool)); // Read once before we update the spline
#endif
	}
}
