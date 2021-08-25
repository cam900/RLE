// MATRIX RLE Compressor/Decompressor
#include<iostream>
#include<fstream>
#include<stdint.h>


using namespace std;


class rle
{
public:
	void compress(bool drle, char* inpath, char* outpath);
	void decompress(bool drle, char* inpath, char* outpath);
	uint64_t read_length(ifstream& fin);
	void write_length(ofstream& fout, uint64_t in);
};


// read_length: Read variable size length value from file
uint64_t rle::read_length(ifstream& fin)
{
	uint8_t in = 0;
	fin.read(reinterpret_cast<char*>(&in), sizeof(uint8_t)); // fetch data from file
	if (in & 0x80) // multi byte length
	{
		uint64_t res = in & 0x7f;
		uint8_t adj = 7;
		while (in & 0x80) // continuous data?
		{
			fin.read(reinterpret_cast<char*>(&in), sizeof(uint8_t)); // fetch next byte
			res += (uint64_t(in & 0x7f) << adj) + (1ULL << adj); // add to result
			adj += 7;
		}
		return res;
	}
	return in;
}


// write_length: Write variable size length value from file
void rle::write_length(ofstream &fout, uint64_t in)
{
	uint8_t outbyte;
	if (in < 0x80) // 0-7f : Single byte
	{
		outbyte = in & 0x7f;
		fout.write(reinterpret_cast<char*>(&outbyte), sizeof(uint8_t));
	}
	else // 80 or larger: Multi byte
	{
		uint64_t tmp = 0x80;
		uint64_t prv = 0x80;
		uint8_t adj = 7;
		int siz = 0;
		while (in >= tmp)
		{
			prv = tmp;
			siz += 1;
			adj += 7;
			tmp += 1ULL << adj;
		}
		in -= prv;
		while (siz)
		{
			outbyte = 0x80 | (in & 0x7f);
			fout.write(reinterpret_cast<char*>(&outbyte), sizeof(uint8_t));
			siz -= 1;
			in = in >> 7;
		}
		outbyte = in & 0x7f;
		fout.write(reinterpret_cast<char*>(&outbyte), sizeof(uint8_t));
	}
}


// compress: Compress file with RLE algorithm
void rle::compress(bool drle, char* inpath, char* outpath)
{
	ifstream fin(inpath, ifstream::binary);

	if (!fin)
		cout << "Failed compressing file" << endl;
	else
	{
		ofstream fout(outpath, ios::binary | ios::out);
		if (fout.is_open())
		{
			uint64_t repval = 0;
			uint8_t curr = 0;
			int16_t prev = -1;
			uint8_t dcur = 0;
			uint8_t dprv = 0;

			// get file length
			fin.seekg(0, ios::end);
			streampos length = fin.tellg();
			fin.seekg(0, ios::beg);

			// Compress
			while (fin.tellg() < length)
			{
				if (drle)
				{
					fin.read(reinterpret_cast<char*>(&dcur), sizeof(uint8_t));
					curr = ((dcur - dprv) + 256) & 0xff;
					dprv = dcur;
				}
				else
				{
					fin.read(reinterpret_cast<char*>(&curr), sizeof(uint8_t));
				}
				if (curr != prev) // new value
				{
					if (repval > 0)
					{
						write_length(fout, repval - 1);
						repval = 0;
					}
					fout.write(reinterpret_cast<char*>(&curr), sizeof(uint8_t));
				}
				else // repeated value
				{
					if (repval <= 0)
					{
						fout.write(reinterpret_cast<char*>(&curr), sizeof(uint8_t));
					}
					repval += 1;
				}
				prev = curr;
			}

			// Flush RLE repeat
			if (repval > 0)
			{
				write_length(fout, repval - 1);
				repval = 0;
			}
			fout.close();
			cout << "Success compressing file" << endl;
		}
		else
			cout << "Failed compressing file" << endl;
		fin.close();
	}
}


// decompress: Decompress already compressed file with RLE algorithm
void rle::decompress(bool drle, char* inpath, char* outpath)
{
	ifstream fin(inpath, ifstream::binary);

	if (!fin)
		cout << "Failed decompressing file" << endl;
	else
	{
		ofstream fout(outpath, ios::binary | ios::out);
		if (fout.is_open())
		{
			uint64_t repval = 0;
			uint8_t curr = 0;
			int16_t prev = -1;
			uint8_t dcur = 0;
			uint8_t buf = 0;

			// get file length
			fin.seekg(0, ios::end);
			streampos length = fin.tellg();
			fin.seekg(0, ios::beg);

			// Decompress
			while (fin.tellg() < length)
			{
				if (drle)
				{
					fin.read(reinterpret_cast<char*>(&dcur), sizeof(uint8_t));
					curr = (dcur + curr) & 0xff;
				}
				else
				{
					fin.read(reinterpret_cast<char*>(&curr), sizeof(uint8_t));
				}
				buf = drle ? dcur : curr;
				fout.write(reinterpret_cast<char*>(&curr), sizeof(uint8_t));
				if (buf == prev) // repeated value
				{
					repval = read_length(fin);
					if (repval > 0)
					{
						while (repval)
						{
							if (drle)
							{
								curr += dcur;
							}
							fout.write(reinterpret_cast<char*>(&curr), sizeof(uint8_t));
							repval--;
						}
					}
				}
				prev = buf;
			}
			fout.close();
			cout << "Success decompressing file" << endl;
		}
		else
			cout << "Failed decompressing file" << endl;
		fin.close();
	}
}


int main(int argc, char** argv)
{
	cout << endl;
	cout << "MATRIX RLE Compressor/Decompressor" << endl;
	cout << "https://github.com/cam900/RLE" << endl;
	cout << endl;
	rle r;
	if (argc == 4) // Normal RLE mode
	{
		if (!_stricmp(argv[1], "-c"))
			r.compress(false, argv[2], argv[3]);
		else if (!_stricmp(argv[1], "-d"))
			r.decompress(false, argv[2], argv[3]);
		else
			cout << "ERROR: Undefined usage!" << endl;
	}
	else if (argc == 5) // Delta RLE mode
	{
		if (_stricmp(argv[1], "-drle"))
			cout << "ERROR: Undefined usage!" << endl;
		else
		{
			if (!_stricmp(argv[2], "-c"))
				r.compress(true, argv[3], argv[4]);
			else if (!_stricmp(argv[2], "-d"))
				r.decompress(true, argv[3], argv[4]);
			else
				cout << "ERROR: Undefined usage!" << endl;
		}
	}
	else
	{
		cout << "Usage: " << argv[0] << " (-drle) (-c/-d) input output" << endl;
		cout << "-drle = Delta RLE mode (optional)" << endl;
		cout << "-c = Compress" << endl;
		cout << "-d = Decompress" << endl;
		cout << "input = Input file name" << endl;
		cout << "output = Output file name" << endl;
	}
	cout << endl;
	system("PAUSE");
	return 0;
}
