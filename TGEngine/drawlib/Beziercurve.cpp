#include "Beziercurve.hpp"

void drawBeziercurve(VertexBuffer* vertices, IndexBuffer* indices, size_t max_vertices, vector<TGVertex>* points) {
    max_vertices -= 2;
    if(points->size() == 0) return;
    if(points->size() == 1)
    {
        vertices->add(points->at(0));
        for(int i = 0; i < 3; i++)
            indices->addIndex(vertices->count_of_points - 1);
        return;
    }
    if(points->size() == 2)
    {
        int firstIndex = vertices->count_of_points;
        TGVertex lastIndex = firstIndex;
        for(float i = 0; i < max_vertices; i++)
        {
            indices->addIndex(firstIndex);
            indices->addIndex(lastIndex);
            vertices->add(math::midpoint(points->at(0), points->at(1), i / (max_vertices - 1));
            indices->addIndex(lastIndex = (vertices->count_of_points - 1));    
        }
        return;
    }
    int startIndex = vertices->count_of_points;
    for(float t = 0; t < max_vertices; t++)
    {
        TGVertex buffer[points->size() - 1];
        for(int i = 0; i < max_vertices - 1; i++)
            buffer[i] = math::midpoint(points->at(i), points->at(i + 1), t / (max_vertices - 1));
        for(int i = max_vertices - 1; i > 2; i--)
            for(int j = 1; j < i - 1; j++)
                buffer[j - 1] = math::midpoint(buffer[j - 1], buffer[j], t / (max_vertices - 1));
        vertices->add(buffer[0]);
    }
    for(int i = 1; i < max_vertices; i++)
    {
        indices->addIndex(startIndex);
        indices->addIndex(startIndex + i - 1);
        indices->addIndex(startIndex + i);
    }
}
