#include "world.h"




////example of parser of .map from rogued editor
//World* World::loadGameMap(const char* filename)
//{
//	FILE* file = fopen(filename, "rb");
//	if (file == NULL) //file not found
//		return NULL;
//
//	sMapHeader header; //read header and store it in the struct
//	fread(&header, sizeof(sMapHeader), 1, file);
//	assert(header.bytes == 1); //always control bad cases!!
//
//
//	//allocate memory for the cells data and read it
//	unsigned char* cells = new unsigned char[header.w*header.h];
//	fread(cells, header.bytes, header.w*header.h, file);
//	fclose(file); //always close open files
//	//create the map where we will store it
//	World* map = new World(header.w, header.h);
//
//	for (int x = 0; x < map->width; x++)
//		for (int y = 0; y < map->height; y++)
//			map->getCell(x, y).type = (eCellType)cells[x + y * map->width];
//
//	delete[] cells; //always free any memory allocated!
//
//	return map;
//}


