#ifndef _MEDIAN_SELECT_HPP_
#define _MEDIAN_SELECT_HPP_

#include <vector>
#include <algorithm>
#include <cmath>

template<class T>
class MedianSelect
{
public:
    typedef std::vector<T> TVector;

    /**
     * Method finds k-th element in points vector.
     * @param unsigned int k
     * @param TVector::iterator iterator to the begining of range of vector to manipulate on
     * @param TVector::iterator iterator to the end of range of vector to manipulate on
     * @return Point found point.
     */
    static T selectKElement(unsigned int k, typename TVector::iterator start, typename TVector::iterator end)
    {
        unsigned int n = end-start;
        if(n<10) // for little size of problem, better solution is to sort and return k-th element
        {
            TVector vec;
            vec.assign(start,end);
            std::sort(vec.begin(),vec.end());
            return vec[k];
        }

        TVector medians(n/5);

        for(unsigned int i = 0;i<(end-start)/5;++i)
        {
           medians[i] = getMedianOf5(i*5,i*5+5,medians); // find median of given range of 5 vec.
        }
        T median = selectKElement(ceil((n/5)/2),medians); // find median of medians.

        TVector lower; // values less than median
        TVector equal; // values equal median
        TVector upper; // values higher than median
   
        typename TVector::const_iterator iter = start;
        typename TVector::const_iterator endIter = end;

        for(;iter!=endIter;++iter)
        {
            if(*iter < median)
            {
                lower.push_back(*iter);
            }
            else if(*iter == median)
            {
                equal.push_back(*iter);
            }
            else
            {
                upper.push_back(*iter);
            }
        }

        if(lower.size() <= k)
            return selectKElement(k,lower);
        else if((lower.size() + equal.size()) <= k)
            return median;
        else
            return selectKElement(k-lower.size()-equal.size(),upper);
    }

    /**
     * Method finds k-th element in points vector.
     * @param unsigned int k
     * @param const PointVector& vector of points to look in. Default is main PointVector
     * @return Point found point.
     */
    static T selectKElement(unsigned int k, TVector& vec)
    {
        return selectKElement(k,vec.begin(),vec.end());
    }
private:

    /**
     * Method returns median from given range of point vector.
     * @param unsigned int index of start element in PointVector
     * @param unsigned int index of end element in PointVector
     * @param PointVector& Reference to vector of points to look in.
     * @return Point Found median.
     */
    static T getMedianOf5(unsigned int start, unsigned int end, TVector& vec)
    {
        bool go=true;
        while(go)
        {
            go=false;
            for (unsigned int i=start; i<end-1; ++i)
                if (vec[i+1]<vec[i])
                {
                    go=true;
                    std::swap(vec[i],vec[i+1]);
                }
        }
        return vec[start+3-1]; // get third position (median)
    }
};

#endif
