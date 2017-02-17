//////////////////////////////////////////////////////////////////////
// ArrayList.h
// Description:		Replacement for STL's vector class
//					ie, easier to debug, safer (array index errors 
//						caught by debug assertions), more flexible
//
// By:				Ben Woodhouse 
//
// Date:			02/2004
//
// Version:			1.3
//
//////////////////////////////////////////////////////////////////////

#pragma once

#define ARRAYLIST_MIN_SIZE 8

namespace Mirage
{
	template <class T>
	class ArrayList 
	{
	public:
		ArrayList()			{
								_size=0;
								minSize=0; 
								maxSize=0; 
								allocDat.shrinkable=true;
								buf=NULL; 
							}
		ArrayList(uint size_in)
							{
								allocDat.shrinkable=true;
								_size=0;
								buf=NULL;
								minSize=0;
								maxSize=0;
								resize(size_in);

							}
		ArrayList(const ArrayList<T>& src)	
		{
			_size=0; buf=NULL; maxSize=0; minSize=0; *this=src;
		}
		virtual ~ArrayList()
							{
								if (buf) delete [] buf;
							}

		ArrayList<T>& operator=(const ArrayList<T>& src)	
		{
			allocDat.shrinkable=src.allocDat.shrinkable;
			resize(src.size());
			for (int a=0;a<_size;a++) buf[a]=src.buf[a];
			return *this;
		}
		void push_back(const T& object)	{add(object);} //for compatibility with Vector
		void add(const T &object)
		{
			if (_size==maxSize)
			{
				T objCopy=object; //create a backup in case the object gets deleted
				resize(_size+1);
				buf[_size-1]=objCopy;
			}
			else 
			{
				_size++;
				buf[_size-1]=object;
			}
		}
		void append(const ArrayList<T>& src)
		{
			insert(_size,src,0,src._size);
		}

		//inserts an object at index I
		int insert(uint i, const T& object)
		{
			assert(i<=_size);
			if (_size==maxSize)
			{
				T objCopy=object;
				resize(_size+1);
				for (uint a=_size-1;a>i;a--)
					buf[a]=buf[a-1];
				buf[i]=objCopy;
			}
			else
			{
				resize(_size+1);
				for (uint a=_size-1;a>i;a--)
					buf[a]=buf[a-1];
				buf[i]=object;
			}
			return i+1;	//return the index of the next point
		}

		int insert(uint i, const ArrayList<T>& src, uint begin, uint end)
		{
			assert(i<=_size);
			assert(begin<=end);
			assert(end<=src.size());
			int cSize=end-begin;
			int destEnd=i+cSize-1;
			if (_size==maxSize)
			{
				ArrayList<T> srcCopy=src;
				resize(_size+cSize);
				for (uint a=_size-1;a>destEnd;a--)
					buf[a]=srcCopy.buf[a-cSize];
				for (uint a=0;a<cSize;a++)
					buf[a+i]=srcCopy.buf[begin+a];
			}
			else
			{
				resize(_size+cSize);
				for (uint a=_size-1;a>destEnd;a--)
					buf[a]=buf[a-cSize];
				for (uint a=0;a<cSize;a++)
					buf[a+i]=src.buf[begin+a];
			}
			return i+cSize;	//return the index of the next point
		}

		int erase(uint i)
		{
			assert(i<_size);
			int sizeMinus1=_size-1;
			for (uint a=i;a<sizeMinus1;a++)
				buf[a]=buf[a+1];
			resize(sizeMinus1);
			return i;
		}

		bool remove(const T &object)
		{
			for (int a=0;a<_size;a++)
			{
				if (*(&buf[a])==*(&object)) 
				{
					erase(a);
					return true;
				}
			}
			return false;
		}

		void allocate(uint newMaxSize)
		{
			if (newMaxSize!=maxSize)
			{
				T* newBuf=new T[newMaxSize];
				int n=min(_size,min(newMaxSize,maxSize));
				for (int a=0;a<n;a++) newBuf[a]=buf[a];
				delete [] buf;
				buf=newBuf;
				maxSize=newMaxSize;
				minSize=newMaxSize;
			}
		}

		void resize(uint newSize)
		{
			if (newSize>maxSize)
			{
				//expand the buffer
				int newMaxSize=max(maxSize, ARRAYLIST_MIN_SIZE);
				while (newMaxSize<newSize)
				{
					newMaxSize=newMaxSize<<1;
				}
				T * newBuf=NULL;
				newBuf=new T[newMaxSize];
				for (int a=0;a<_size;a++) newBuf[a]=buf[a];
				if (buf) delete [] buf;
				buf=newBuf;
				maxSize=newMaxSize;
				minSize=maxSize>>1;
			}
			else if (allocDat.shrinkable)
			{
				if (newSize==0)
				{
					minSize=0;
					maxSize=0;
					delete [] buf;
					buf=NULL;
				}
				else if (newSize<_size)
				{
					if (newSize<minSize && minSize>ARRAYLIST_MIN_SIZE)
					{
						//shrink the buffer
						int newMinSize=max(minSize,ARRAYLIST_MIN_SIZE);
						while (newMinSize>newSize)
						{
							newMinSize=newMinSize>>1;
						}
						int newMaxSize=newMinSize<<1;

						T * newBuf=new T[newMaxSize];
						for (int a=0;a<newSize;a++) newBuf[a]=buf[a];
						if (buf!=NULL) delete [] buf;
						buf=newBuf;
						maxSize=newMaxSize;
						minSize=newMinSize;
					}
				}
			}
			_size=newSize;
		}
		const T& getConst(uint i) const 
									{
										assert(i<_size);
										return buf[i];
									}
		T& get(uint i) const		{
										assert(i<_size);
										return buf[i];
									}
		T& operator[](uint i) const	{
										assert(i<_size);
										return buf[i];
									}

		uint size() const			{return _size;}

		T& last()					{
										assert(_size>0);
										return buf[_size-1];
									}
		T& first()					{
										assert(_size>0);
										return buf[0];
									}

		void clear()				{	resize(0);}

		T sum(T initialVal)			{ T rv=initialVal; for (int a=0;a<_size;a++) rv+=buf[a]; return rv; }
		T sum()						{ T rv; for (int a=0;a<_size;a++) rv+=buf[a]; return rv; }

		//allocation flags
		void allocSetShrinkable(bool shrinkable)	{allocDat.shrinkable=shrinkable;}

		//reorderings
		void randomiseOrder(int seed=-1)
		{
			ArrayList<T> rand;
			while (_size>0)
			{
				int i=(seed==-1) ? Mirage::random(_size) : Mirage::random(_size,seed);
				rand.add(get(i));
				erase(i);
			}
			*this=rand;
		}

		void filter(const ArrayList<T>& src) //merges the common elements of both arraylists into this arraylist
		{
			int newSize=_size;
			for (int a=0;a<_size;a++)
			{
				bool common=false;
				for (int b=0;b<src._size;b++)
				{
					if (src.buf[b]==buf[a])
					{
						common=true;
						break;
					}
				}
				if (!common) //remove the element
				{
					newSize--;
					buf[a]=buf[newSize];
				}
			}
			resize(newSize);
		}

		void makeUnique()
		{
			sort();
			unique();
		}
		void sort()	{quickSort(buf,_size);}
		
		void unique() //removes adjacent duplicate elements
		{
			int i=1;
			for (int a=1;a<_size;a++)
			{
				if (!(buf[a]==buf[a-1]))
				{
					if (i!=a)
					{
						buf[i]=buf[a];
					}
					i++;
				}
			}
			resize(i);
		}

	private:
		void quickSort(T *array, int size)
		{
			int i;
			int iL,iR;
			int pivot;
			int leftSize;
			T temp;

			if (size<3) //stopping condition
			{
				if (size==2)
				{
					if (array[1]<array[0])
					{
						temp=array[0];
						array[0]=array[1];
						array[1]=temp;
					}
				}
			}
			else
			{
				pivot=selectPivot(array);

				//calculate the size of the lefthand portion 
				leftSize=0;
				for (i=0;i<size;i++)
				{
					if (array[i]<array[pivot]) 
						leftSize++;
				}

				//move the pivot to its final position in the array
				if (pivot!=leftSize)
				{
					temp=array[leftSize];
					array[leftSize]=array[pivot];
					array[pivot]=temp;
					pivot=leftSize;
				}

				//move the elements to the left or right side of the pivot
				iL=0;
				iR=leftSize+1;
				while (iL<pivot && iR<size)
				{
					if (!(array[iL]<array[pivot]) && array[iR]<array[pivot])
					{
						//swap the elements at iL and iR
						temp=array[iL]; 
						array[iL]=array[iR];
						array[iR]=temp;
						iL++; iR++;
					}
					else
					{
						if (array[iL]<array[pivot]) iL++;
						if (!(array[iR]<array[pivot])) iR++;
					}
				}
				//quicksort the left and right sides
				quickSort(&array[0], leftSize);
				quickSort(&array[leftSize+1], size-leftSize-1);
			}
		}

		int selectPivot(T *array)
		{
			if (array[0]<array[1])
			{
				if (array[2]<array[0])
					return 0;
				else if (array[1]<array[2])
					return 1;
				else 
					return 2;
			}
			else
			{
				if (array[2]<array[1])
					return 1;
				else if (array[0]<array[2])
					return 0;
				else
					return 2;
			}
			return 0; 
		}

		T *buf;
		uint _size;
		uint maxSize;
		uint minSize;
		struct 
		{
			bool shrinkable;
		}allocDat;
	};

}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

