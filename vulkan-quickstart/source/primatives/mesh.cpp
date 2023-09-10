#include "primatives/mesh.h"
#include <fstream>
#include <sstream>

bool vulkan::primatives::mesh::load(std::string filename)
{
    std::vector<vector3> temp;

    std::ifstream source(filename);
    if(source.is_open())
    {
        for(std::string value; getline(source, value); )
        {
            std::stringstream stream(value);
		    std::string type, word;
	
            std::vector<std::string> words;

            if(stream >> type)
            {
                if(type == "v")
                {
                    while(stream >> word) words.push_back(word);

                    if(words.size() >= 3)
                    {
                        vector3 v;
                        float *xyz[] = { &v.x, &v.y, &v.z };
                        int index = 0;

                        for(std::vector<std::string>::iterator it = words.begin(); it < words.begin() + 3; ++it)
                        {
                            *xyz[index++] = std::atof((*it).c_str());
                        }

                        temp.push_back(v);
                    }
                }
                else if(type =="f")
                {
                    // f v1/vt1/vn1
                    // vertex, texture, normal

                    while(stream >> word) words.push_back(word);

                    if(words.size() >= 3)
                    {
                        int xyz[] = { 0, 0, 0 };
                        int index = 0;

                        for(std::vector<std::string>::iterator it = words.begin(); it < words.begin() + 3; ++it)
                        {
                            std::stringstream data(*it);
                            for(std::string token; getline(data, token, '/'); )
                            {
                                xyz[index++] = atoi(token.c_str());
                                break;
                            };
                        }
                        
                        if((xyz[0] > 0)&&(xyz[1] > 0)&&(xyz[2] > 0))
                        {
                            vector3 v1 = temp[xyz[0] - 1];
                            vector3 v2 = temp[xyz[1] - 1];
                            vector3 v3 = temp[xyz[2] - 1];
                                                
                            vertices.push_back(vertex(v1));
                            vertices.push_back(vertex(v2));
                            vertices.push_back(vertex(v3));
                        }
                    }
                }
            }
        }

        source.close();

        return vertices.size() > 0;
    }

    return false;
}