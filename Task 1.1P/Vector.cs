using System;
using System.Collections.Generic;
using System.Text;

namespace SIT221_Library
{
    public class Vector<T>
    {
        // This constant determines the default number of elements in a newly created vector.
        // It is also used to extended the capacity of the existing vector
        private const int DEFAULT_CAPACITY = 10;

        // This array represents the internal data structure wrapped by the vector class.
        // In fact, all the elements are to be stored in this private  array. 
        // You will just write extra functionality (methods) to make the work with the array more convenient for the user.
        private T[] data;

        // This property represents the number of elements in the vector
        public int Count { get; private set; } = 0;

        // This property represents the maximum number of elements (capacity) in the vector
        public int Capacity
        {
            get { return data.Length; }
        }

        // This is an overloaded constructor
        public Vector(int capacity)
        {
            data = new T[capacity];
        }

        // This is the implementation of the default constructor
        public Vector() : this(DEFAULT_CAPACITY) { }

        // An Indexer is a special type of property that allows a class or structure to be accessed the same way as array for its internal collection. 
        // For example, introducing the following indexer you may address an element of the vector as vector[i] or vector[0] or ...
        public T this[int index]
        {
            get
            {
                if (index >= Count || index < 0) throw new IndexOutOfRangeException();
                return data[index];
            }
            set
            {
                if (index >= Count || index < 0) throw new IndexOutOfRangeException();
                data[index] = value;
            }
        }

        // This private method allows extension of the existing capacity of the vector by another 'extraCapacity' elements.
        // The new capacity is equal to the existing one plus 'extraCapacity'.
        // It copies the elements of 'data' (the existing array) to 'newData' (the new array), and then makes data pointing to 'newData'.
        private void ExtendData(int extraCapacity)
        {
            T[] newData = new T[Capacity + extraCapacity];
            for (int i = 0; i < Count; i++) newData[i] = data[i];
            data = newData;
        }

        // This method adds a new element to the existing array.
        // If the internal array is out of capacity, its capacity is first extended to fit the new element.
        public void Add(T element)
        {
            if (Count == Capacity) ExtendData(DEFAULT_CAPACITY);
            data[Count] = element;
            Count++;
        }

        // This method searches for the specified object and returns the zero‐based index of the first occurrence within the entire data structure.
        // This method performs a linear search; therefore, this method is an O(n) runtime complexity operation.
        // If occurrence is not found, then the method returns –1.
        // Note that Equals is the proper method to compare two objects for equality, you must not use operator '=' for this purpose.
        public int IndexOf(T element)
        {
            for (var i = 0; i < Count; i++)
            {
                if (data[i].Equals(element)) return i;
            }
            return -1;
        }

        // This method inserts a given T element into the data collection at provided index
        public void Insert(int index, T element)
        {
            // Exception is thrown if index is outside of the array range
            if (index > Count || index < 0) throw new IndexOutOfRangeException("Index is out of array range");       
            // Increase capacity if full
            if (Count == Capacity) ExtendData(DEFAULT_CAPACITY);
            // If the index equals the count, the item is added to the end of the Vector<T>
            if (index == Count)
            {
                Add(element);
            }
            // Otherwise, loop through the vector find where the index is located and insert the element
            else
            {;
                for (int i = Count; i > index; i--)
                {
                    data[i] = data[i - 1];
                }
                // Insert new element at index
                data[index] = element;
                Count++;
            }
        }

        // This method sets Count to 0 and removes all elements from vector without changing its capacity
        public void Clear()
        {
            // Set Count to 0
            Count = 0;
        }

        // This method determines whether a specific T element exists within the data list
        public bool Contains(T element)
        {
            // Iterate through the list and return true if the specified T element exists, otherwise return false
            for (int i = 0; i < Count; i++)
            {
                if (data[i].Equals(element)) return true;
            }
            return false;
        }

        // This method removes the first occurrence of the specified element from the data list
        public bool Remove(T element)
        {
            // Iterate through the list to find the first occurrence of T element
            if (Contains(element))
            {
                // Use IndexOf and RemoveAt methods to find index and remove it from the list, then return true 
                RemoveAt(IndexOf(element));
                return true;
            }
            // Return false if the element is not found in the list
            else return false;
        }

        // This method removes the element located at a specified index and shuffles list to close gap
        public void RemoveAt(int index)
        {
            // Throw exception if index value is invalid
            if (index >= Count || index < 0) throw new IndexOutOfRangeException("Index is out of array range");  
            // Shuffle all elements from the index onwards to remove the element
            for (int i = index; i < Count; i++)
            {
                data[i] = data[i + 1];
            }
            Count--;
        }

        // This method returns a string that represents the current object
        public override string ToString()
        {
            // Return an empty string [] if count is 0 and there are no elements in the list
            if (Count == 0) return "[]";
            
            // Create a new empty string to build the list into
            StringBuilder sb = new StringBuilder();
            
            for (int i = 0; i < Count; i++)
            {
                // Format the list to begin and end with square brackets
                if (i == 0) 
                    sb.Append($"[{data[i]},");
                else if (i == Count - 1) 
                    sb.Append($"{data[i]}]");
                // Display all other elements of the list
                else
                    sb.Append($"{data[i]},");
            }
            // Returns a formatted string
            return sb.ToString();
        }
    }
}
