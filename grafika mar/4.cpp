#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>
#include <string>

int get_index(int x, int y, int width)
{
    return x + width * y;
}

int calc_size(int width, int height)
{
    return width * height;
}

void clear_image(std::vector<std::tuple<float, float, float>> &image,
                 std::tuple<float, float, float> &color,
                 int width, int height)
{
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            image[get_index(x, y, width)] = color;
        }
    }
}

// dynamiczny xor
void xor_draw(std::vector<std::tuple<float, float, float>> &image,
              int width, int height, int t)
{
    int index = 0;

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            unsigned char value = ((x + t) ^ (y - t)) & 255;

            float r = value / 255.0f;
            float g = ((value << 1) & 255) / 255.0f;
            float b = (value ^ 128) / 255.0f;

            image[index] = std::make_tuple(r, g, b);
            index++;
        }
    }
}

void save_image(std::vector<std::tuple<float, float, float>> &image,
                int width, int height, std::string filename)
{
    std::ofstream out(filename);

    out << "P3\n" << width << " " << height << "\n255\n";

    for (int i = 0; i < (width * height); ++i)
    {
        auto color = image[i];

        out << int(std::get<0>(color) * 255.0f) << " "
            << int(std::get<1>(color) * 255.0f) << " "
            << int(std::get<2>(color) * 255.0f) << "\n";
    }

    out.close();
}

int main()
{
    int width = 640;
    int height = 360;
    int FRAMES = 60;

    std::vector<std::tuple<float, float, float>> image;
    image.resize(calc_size(width, height));

    std::cout << "Generating animation (" << FRAMES << " frames)..." << std::endl;

    for (int frame = 0; frame < FRAMES; ++frame)
    {
        int t = frame * 4;

        xor_draw(image, width, height, t);

        std::string filename = "frame_" + std::to_string(frame) + ".ppm";
        save_image(image, width, height, filename);

        std::cout << "Saved: " << filename << std::endl;
    }

    std::cout << "Done!" << std::endl;

    return 0;
}