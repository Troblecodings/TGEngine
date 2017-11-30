#include "Circle.hpp"

void drawCircle(Vertex pos, float radius, int max_verticies, std::vector<Vertex>* vert) {
	float angle = PIx2 / max_verticies;
	uint32_t siz = vert->size();
    #ifndef USE_INDEX_BUFFER
	vert->resize(siz + max_verticies*3 + 3);
	float l_x;
	float l_y;
    #else
	uint32_t ind_siz = indicies.size();
	indicies.resize(ind_siz + max_verticies * 3 + 3);
	vert->resize(siz + max_verticies + 2);
	(*vert)[siz] = pos;
    #endif

	for (size_t i = 0; i < max_verticies + 1; i++)
	{
		float r_angle = angle*i;
		float x = pos.position.x + radius * cos(r_angle);
		float y = pos.position.y + radius * sin(r_angle);
        #ifndef USE_INDEX_BUFFER 
		if (i != 0) {
			(*vert)[siz + (i * 3) + 1] = {
				{
					l_x,l_y,1
				},
				pos.color
			};
		}
		else {
			l_x = x;
			l_y = y;
			continue;
		}
		l_x = x;
		l_y = y;
		(*vert)[siz + (i * 3) + 2] = {
			{
				x,y,1
			},
			pos.color
	    };
		(*vert)[siz + (i * 3)] = pos;
        #else
		(*vert)[siz + i  + 1] = {
			{
				x, y, 1
			},
			pos.color
		};
		if (i == 0)continue;
		indicies[ind_siz + i * 3] = siz;
		indicies[ind_siz + i * 3 + 1] = siz + i;
		indicies[ind_siz + i * 3 + 2] = siz + i + 1;
		#endif
		
	}
}