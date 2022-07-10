// MATRIX RLE Compressor/Decompressor

#include <string>

#include "librle/librle.hpp"

int main(int argc, char** argv)
{
	std::cout << std::endl;
	std::cout << "MATRIX RLE Compressor/Decompressor" << std::endl;
	std::cout << "https://github.com/cam900/RLE" << std::endl;
	std::cout << std::endl;
	librle r;
	librle::librle_status ret;
	if (argc == 4) // Normal RLE mode
	{
		if (!_stricmp(argv[1], "-c"))
		{
			uint64_t original_size = 0, compressed_size = 0;
			switch (r.compress(false, argv[2], argv[3], &original_size, &compressed_size))
			{
				case librle::librle_status::LIBRLE_SUCCESS:
					std::cout << "Succeed to compressing file!" << std::endl;
					std::cout << "Original size: " << std::to_string(original_size) << std::endl;
					std::cout << "Compressed size: " << std::to_string(compressed_size) << " (compression raito: " << std::to_string(100.0 * double(compressed_size) / double(original_size)) << "%)" << std::endl;
					break;
				case librle::librle_status::LIBRLE_FAILED_TO_OPEN_SOURCE:
					std::cout << "Failed to open " << argv[2] << "!" << std::endl;
					break;
				case librle::librle_status::LIBRLE_FAILED_TO_OPEN_DESTINATION:
					std::cout << "Failed to open " << argv[3] << "!" << std::endl;
					break;
				default:
					std::cout << "Unknown error in compressing file!" << std::endl;
					break;
			}
		}
		else if (!_stricmp(argv[1], "-d"))
		{
			uint64_t original_size = 0, decompressed_size = 0;
			switch (r.decompress(false, argv[2], argv[3], &original_size, &decompressed_size))
			{
				case librle::librle_status::LIBRLE_SUCCESS:
					std::cout << "Succeed to decompressing file!" << std::endl;
					std::cout << "Original size: " << std::to_string(original_size) << std::endl;
					std::cout << "Decompressed size: " << std::to_string(decompressed_size) << " (compression raito: " << std::to_string(100.0 * double(original_size) / double(decompressed_size)) << "%)" << std::endl;
					break;
				case librle::librle_status::LIBRLE_FAILED_TO_OPEN_SOURCE:
					std::cout << "Failed to open " << argv[2] << "!" << std::endl;
					break;
				case librle::librle_status::LIBRLE_FAILED_TO_OPEN_DESTINATION:
					std::cout << "Failed to open " << argv[3] << "!" << std::endl;
					break;
				default:
					std::cout << "Unknown error in decompressing file!" << std::endl;
					break;
			}
		}
		else
			std::cout << "ERROR: Undefined usage!" << std::endl;
	}
	else if (argc == 5) // Delta RLE mode
	{
		if (_stricmp(argv[1], "-drle"))
			std::cout << "ERROR: Undefined usage!" << std::endl;
		else
		{
			if (!_stricmp(argv[2], "-c"))
			{
				uint64_t original_size = 0, compressed_size = 0;
				switch (r.compress(true, argv[3], argv[4], &original_size, &compressed_size))
				{
					case librle::librle_status::LIBRLE_SUCCESS:
						std::cout << "Succeed to compressing file!" << std::endl;
						std::cout << "Original size: " << std::to_string(original_size) << std::endl;
						std::cout << "Compressed size: " << std::to_string(compressed_size) << " (compression raito: " << std::to_string(100.0 * double(compressed_size) / double(original_size)) << "%)" << std::endl;
						break;
					case librle::librle_status::LIBRLE_FAILED_TO_OPEN_SOURCE:
						std::cout << "Failed to open " << argv[3] << "!" << std::endl;
						break;
					case librle::librle_status::LIBRLE_FAILED_TO_OPEN_DESTINATION:
						std::cout << "Failed to open " << argv[4] << "!" << std::endl;
						break;
					default:
						std::cout << "Unknown error in compressing file!" << std::endl;
						break;
				}
			}
			else if (!_stricmp(argv[2], "-d"))
			{
				uint64_t original_size = 0, decompressed_size = 0;
				switch (r.decompress(true, argv[3], argv[4], &original_size, &decompressed_size))
				{
					case librle::librle_status::LIBRLE_SUCCESS:
						std::cout << "Succeed to decompressing file!" << std::endl;
						std::cout << "Original size: " << std::to_string(original_size) << std::endl;
						std::cout << "Decompressed size: " << std::to_string(decompressed_size) << " (compression raito: " << std::to_string(100.0 * double(original_size) / double(decompressed_size)) << "%)" << std::endl;
						break;
					case librle::librle_status::LIBRLE_FAILED_TO_OPEN_SOURCE:
						std::cout << "Failed to open " << argv[3] << "!" << std::endl;
						break;
					case librle::librle_status::LIBRLE_FAILED_TO_OPEN_DESTINATION:
						std::cout << "Failed to open " << argv[4] << "!" << std::endl;
						break;
					default:
						std::cout << "Unknown error in decompressing file!" << std::endl;
						break;
				}
			}
			else
				std::cout << "ERROR: Undefined usage!" << std::endl;
		}
	}
	else
	{
		std::cout << "Usage: " << argv[0] << " (-drle) (-c/-d) input output" << std::endl;
		std::cout << "-drle = Delta RLE mode (optional)" << std::endl;
		std::cout << "-c = Compress" << std::endl;
		std::cout << "-d = Decompress" << std::endl;
		std::cout << "input = Input file name" << std::endl;
		std::cout << "output = Output file name" << std::endl;
	}
	std::cout << std::endl;
	system("PAUSE");
	return 0;
}
