#ifndef _BRDF_BECKMANN_
#define _BRDF_BECKMANN_

#include "brdf.h"

class BrdfBeckmann : public Brdf
{
public:
    virtual float eval(const vec3& V, const vec3& L, const float alpha, float& pdf) const
    {
        if (V.z <= 0)
        {
            pdf = 0;
            return 0;
        }

        // masking
        const float LambdaV = lambda(alpha, V.z);

        // shadowing
        float G2;
        if (L.z <= 0.0f)
            G2 = 0;
        else
        {
            const float LambdaL = lambda(alpha, L.z);
            G2 = 1.0f/(1.0f + LambdaV + LambdaL);
        }

        // D
        const vec3 H = normalize(V + L);
        const float slopex = H.x/H.z;
        const float slopey = H.y/H.z;
        float D = expf(-(slopex*slopex + slopey*slopey)/(alpha*alpha)) / (3.14159f * alpha*alpha * H.z*H.z*H.z*H.z);

        pdf = fabsf(D * H.z / 4.0f / dot(V, H));
        float res = D * G2 / 4.0f / V.z;

        return res;
    }

    virtual vec3 sample(const vec3& V, const float alpha, const float U1, const float U2) const
    {
        const float phi = 2.0f*3.14159f * U1;
        const float r = alpha*sqrtf(-logf(U2));
        const vec3 N = normalize(vec3(r*cosf(phi), r*sinf(phi), 1.0f));
        const vec3 L = -V + 2.0f * N * dot(N, V);
        return L;
    }

private:
    float lambda(const float alpha, const float cosTheta) const
    {
        const float a = 1.0f / alpha / tanf(acosf(cosTheta));
        return (cosTheta < 1.0f) ? (1.0f - 1.259f*a + 0.396f*a*a) / (3.535f*a + 2.181f*a*a) : 0.0f;
    }
};

#endif
