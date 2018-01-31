#include <field_renderer.h>

namespace FD = FieldDimension;

void FieldRenderer::render(char* config, bool we_are_blue) {

	//Field bounds
	render_rect(FD::field_bounds, we_are_blue ? blue : red);

	//Switch
	render_rect(FD::Switch::boom, grey);
	render_rect(FD::Switch::left_plate, (we_are_blue == (config[0] == 'L')) ? blue : red);
	render_rect(FD::Switch::right_plate, (we_are_blue == (config[0] == 'R')) ? blue : red);

	//Scale
	render_rect(FD::Scale::boom, grey);
	render_rect(FD::Scale::left_plate, (we_are_blue == (config[1] == 'L')) ? blue : red);
	render_rect(FD::Scale::right_plate, (we_are_blue == (config[1] == 'R')) ? blue : red);
}

void FieldRenderer::render_rect(cv::Rect2f input, cv::Point3f rgb) {
	glLineWidth(2);
	glColor3f(rgb.x, rgb.y, rgb.z);
	glBegin(GL_LINES);

	glVertex2f(input.tl().x, input.tl().y);
	glVertex2f(input.br().x, input.tl().y);

	glVertex2f(input.br().x, input.tl().y);
	glVertex2f(input.br().x, input.br().y);

	glVertex2f(input.br().x, input.br().y);
	glVertex2f(input.tl().x, input.br().y);

	glVertex2f(input.tl().x, input.br().y);
	glVertex2f(input.tl().x, input.tl().y);
	glEnd();
}
