#ifndef NELDER_MEAD_H
#define NELDER_MEAD_H

void mov(float* r, const float* v, int dim)
{
    for (int i = 0; i < dim; ++i)
        r[i] = v[i];
}

void set(float* r, const float v, int dim)
{
    for (int i = 0; i < dim; ++i)
        r[i] = v;
}

void add(float* r, const float* v, int dim)
{
    for (int i = 0; i < dim; ++i)
        r[i] += v[i];
}

// Downhill simplex solver:
// http://en.wikipedia.org/wiki/Nelder%E2%80%93Mead_method#One_possible_variation_of_the_NM_algorithm
// using the termination criterion from Numerical Recipes in C++ (3rd Ed.)
// 不断变换LTC矩阵中的三个元素m11、m22、m13，来迭代产生小于tolerance的，LTC和BRDF的球面分布的误差
template<int DIM, typename FUNC>
float NelderMead(
    float* pmin, const float* start, float delta, float tolerance, int maxIters, FUNC objectiveFn)
{
    // standard coefficients from Nelder-Mead
    const float reflect  = 1.0f;
    const float expand   = 2.0f;
    const float contract = 0.5f;
    const float shrink   = 0.5f;

    typedef float point[DIM];
    const int NB_POINTS = DIM + 1;

    point s[NB_POINTS];
    float f[NB_POINTS];

    // initialise simplex
    mov(s[0], start, DIM);
    for (int i = 1; i < NB_POINTS; i++)
    {
        mov(s[i], start, DIM);
        s[i][i - 1] += delta;	//加0.05
    }

    // evaluate function at each point on simplex
    for (int i = 0; i < NB_POINTS; i++)
        f[i] = objectiveFn(s[i]);	//f[i]是LTC和BRDF在某个观察方向以及粗糙度下球面分布的差异，不同的f[i]对应不同的矩阵的3个元素m11、m22、m13

    int lo = 0, hi, nh;

    for (int j = 0; j < maxIters; j++)
    {
        // find lowest, highest and next highest，找出第几个f[i]是最小的、最大的和次大的（点NB_POINTS的序号）
        lo = hi = nh = 0;
        for (int i = 1; i < NB_POINTS; i++)
        {
            if (f[i] < f[lo])
                lo = i;
            if (f[i] > f[hi])
            {
                nh = hi;
                hi = i;
            }
            else if (f[i] > f[nh])
                nh = i;
        }

        // stop if we've reached the required tolerance level
        float a = fabsf(f[lo]);
        float b = fabsf(f[hi]);
        if (2.0f*fabsf(a - b) < (a + b)*tolerance)
            break;

        // compute centroid (excluding the worst point)
        point o;
        set(o, 0.0f, DIM);
        for (int i = 0; i < NB_POINTS; i++)
        {
            if (i == hi) continue;
            add(o, s[i], DIM);	//把除了最高误差之外的点对应的矩阵元素加到o里
        }

        for (int i = 0; i < DIM; i++)
            o[i] /= DIM;		//求均值

        // reflection
        point r;
        for (int i = 0; i < DIM; i++)
            r[i] = o[i] + reflect*(o[i] - s[hi][i]);	//点r中存储的是除了最高误差只玩的点的矩阵元素的均值，加上均值到最高误差的矩阵元素的差值，其实结果就是最高误差的矩阵元素

        float fr = objectiveFn(r);
        if (fr < f[nh])
        {
            if (fr < f[lo])
            {
                // expansion
                point e;
                for (int i = 0; i < DIM; i++)
                    e[i] = o[i] + expand*(o[i] - s[hi][i]);

                float fe = objectiveFn(e);
                if (fe < fr)
                {
                    mov(s[hi], e, DIM);
                    f[hi] = fe;
                    continue;
                }
            }

            mov(s[hi], r, DIM);
            f[hi] = fr;
            continue;
        }

        // contraction
        point c;
        for (int i = 0; i < DIM; i++)
            c[i] = o[i] - contract*(o[i] - s[hi][i]);

        float fc = objectiveFn(c);
        if (fc < f[hi])
        {
            mov(s[hi], c, DIM);
            f[hi] = fc;
            continue;
        }

        // reduction
        for (int k = 0; k < NB_POINTS; k++)
        {
            if (k == lo) continue;
            for (int i = 0; i < DIM; i++)
                s[k][i] = s[lo][i] + shrink*(s[k][i] - s[lo][i]);
            f[k] = objectiveFn(s[k]);
        }
    }

    // return best point and its value
    mov(pmin, s[lo], DIM);
    return f[lo];	//返回最低误差
}

#endif // NELDER_MEAD_H
