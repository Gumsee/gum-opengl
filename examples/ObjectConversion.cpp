#include "Maths/ColorFunctions.h"
#include <gum-maths.h>

int main(int argc, char** argv)
{
	std::cout << "#" << std::endl;
	std::cout << "# Color Conversion Testing:" << std::endl;
	std::cout << "#" << std::endl;

	vec3 rgb(255, 45, 77);

	std::cout << "RGB: " << rgb.toString() << std::endl;
	std::cout << "HSV: " << Gum::Maths::RGBToHSV(rgb).toString() << std::endl;

	return 0;
};
