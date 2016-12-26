package com.company;

import java.util.*;
import java.lang.Math;

final class TopKeeper<T> implements Iterable<T> {
    TopKeeper(int m) {
        topElems = new ArrayList<T>();
        medians = new ArrayList<T>();
        size = m;
        curSize = m;
    }

    TopKeeper(int m, Comparator<T> tComp) {
        this(m);
        hasComparator = true;
        tComparator = tComp;
    }

    private void tsort(List<T> list) {
        if (hasComparator == true) {
            Collections.sort(list, tComparator);
        } else {
            Collections.sort(list, (T obj1, T obj2) ->
                {return ((Comparable) obj1).compareTo(obj2);});
        }
    }

    private void medians_sort(int leftBound, int rightBound){
        boolean hasSwapped = true;
        int numOfSorted = 0;
        while (hasSwapped) {
            hasSwapped = false;
            numOfSorted++;
            for (int j = leftBound; j < rightBound - numOfSorted; j++) {
                if (tcompare(medians.get(j), medians.get(j + 1)) > 0) {
                    Collections.swap(medians, j, j + 1);
                    hasSwapped = true;
                }
            }
        }
    }

    public T median_of_medians(int begin, int end) {
        int numOfMedians = end - begin;

        while (numOfMedians > 5) {
            numOfMedians = 0;
            for (int segBeg = begin; segBeg < end; segBeg += 5) {
                int segEnd = Math.min(segBeg + 5, end);
                medians_sort(segBeg, segEnd);
                medians.set(numOfMedians++, medians.get((segEnd + segBeg) / 2));
            }
            end = begin + numOfMedians;
        }

        return medians.get(numOfMedians / 2);
    }

    private int tcompare(T obj1, T obj2) {
        if (hasComparator == true) {
            return tComparator.compare(obj1, obj2);
        }
        return ((Comparable)obj1).compareTo(obj2);
    }

    private int partition(T privot, int leftBound, int rightBound) {
        int i = leftBound;
        for (int j = leftBound; j < rightBound; j++) {
            if (tcompare(privot, topElems.get(j)) > 0) {
                if (j > i) {
                    Collections.swap(topElems, i, j);
                }
                i++;
            }
        }
        return i;
    }

    private void make_min_order(int leftBound, int rightBound) {
        medians = new ArrayList<T>(topElems);

        int k = partition(median_of_medians(leftBound, rightBound), leftBound, rightBound);

        if (size < k) {
            make_min_order(leftBound, k);
        } else if (size > k){
            make_min_order(k + 1, rightBound);
        } else {
            return;
        }
    }

    private void forbid_add() {
        if (forbidAddOper == false) {
            forbidAddOper = true;
            for (int i = (2 * size) - 1; i >= size; i--) {
                topElems.remove(i);
            }
            tsort(topElems);
        }
    }

    public void add(T elem) {
        if (forbidAddOper == true) {
            throw new RuntimeException();
        }

        if (topElems.size() == 2 * size) {
            topElems.set(curSize, elem);
            curSize++;
        } else {
            topElems.add(elem);
            return;
        }

        if (curSize == 2 * size) {
            make_min_order(0, 2 * size);
            curSize = size;
        }
    }

    public T next() {
        forbid_add();
        if (size == 0) {
            throw new RuntimeException();
        }
        return topElems.get(size - 1);
    }

    public List<T> toList() {
       return topElems.subList(0, size);
    }

    public void pop() {
        forbid_add();
        if (size == 0) {
            throw new RuntimeException();
        }
        size--;
    }

    @Override
    public Iterator<T> iterator() {
        forbid_add();
        return topElems.iterator();
    }

    public int size() {
        return size;
    }

    private ArrayList<T> topElems;
    private ArrayList<T> medians;
    private int size = 0;
    private int curSize = 0;
    private Comparator<T> tComparator;
    private boolean hasComparator = false;
    private boolean forbidAddOper = false;
}

public class Main {
    public static void main(String[] args) {
    }
}
