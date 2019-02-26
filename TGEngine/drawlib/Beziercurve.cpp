#include "Beziercurve.hpp"

static TGVertex midpoint(TGVertex a, TGVertex b, float t)
{
    return {
        b.position + (a.position - b.position) * t,
        math::mixColor(a.color, b.color, t),
        b.uv + (a.ub - b.uv) * t,
        COLOR_ONLY
    };
}

void drawBeziercurve(VertexBuffer* vertices, IndexBuffer* indices, size_t max_vertices, vector<TGVertex>* points) {
    if(points->size() == 0) return;
    if(points->size() == 1)
    {
        vertices->add(points->at(0));
        return;
    }
    if(points->size() == 2)
    {
        for(float i = 0; i < max_vertices; i++)
            vertices->add(midpoint(points->at(0), points->at(1), i / (max_vertices - 1));
        return;
    }
    for(float t = 0; t < max_vertices; t++)
    {
        TGVertex buffer[points->size() - 1];
        for(int i = 0; i < max_vertices - 1; i++)
            buffer[i] = midpoint(points->at(i), points->at(i + 1), t / (max_vertices - 1));
        for(int i = max_vertices - 1; i > 2; i--)
            for(int j = 1; j < i - 1; j++)
                buffer[j - 1] = midpoint(buffer[j - 1], buffer[j], t / (max_vertices - 1));
        vertices->add(buffer[0]);
        indices->addIndex(vertices->count_of_points - 1);
    }
}