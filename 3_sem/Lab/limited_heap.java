package com.company;

import java.util.*;
import sun.reflect.generics.scope.ClassScope;

class Test_LimitedHeap<T> {
    public static void init() {
        failCount = 0;
        successCount = 0;
    }

    public void expectSuccess(List<T> stream, int m, List<T> ans) {
        LimitedHeap<T> limHeap = new LimitedHeap<>(m);
        for (T i : stream) {
            limHeap.add(i);
        }
        List<T> mMin = new ArrayList<T>();
        mMin = limHeap.toList();

        Collections.sort(ans, Collections.reverseOrder());

        if(!mMin.equals(ans)) {
            report(mMin, ans, false);
        } else {
            successCount++;
        }
    }

    private void report(List<T> a, List<T> b, boolean got) {
        failCount++;
        System.out.println( "For " + a + " and " + b + ",
                           expected: " + !got + " ,actual: " + got);
    }


    public static void reportSummary() {
        if (successCount == 0) {
            System.out.println("Summary: all " + failCount +
                    " tests failed!");
        } else if (failCount == 0) {
            System.out.println("Summary: all " + successCount +
                    " tests succeeded!");
        } else {
            System.out.println("Summary: " + successCount +
                    " tests succeeded, " + failCount +
                    " tests failed.");
        }
    }

    private static int failCount;
    private static int successCount;
}

class LimitedHeap<T> implements Iterable<T> {
    LimitedHeap(int m) {
       init(m);
    }

    LimitedHeap(int m, Comparator<T> tComp) {
        hasComparator = true;
        tComparator = tComp;
        init(m);
    }

    private void init(int m) {
        maxSize = m;
        heap = new PriorityQueue<T>(1, (obj1,  obj2) -> tcompare(obj2, obj1));
    }

    private int tcompare(T obj1, T obj2) {
        if (hasComparator == true) {
            return tComparator.compare(obj1, obj2);
        }
        return ((Comparable)obj1).compareTo(obj2);
    }

    public void add(T elem) {
        if (heap.size() == maxSize) {
            if (tcompare(elem, heap.peek()) >= 0) {
                return;
            }
            heap.poll();
        }
        heap.add(elem);
    }

    public T top() {
        if (heap.size() == 0) {
            throw new RuntimeException();
        }
        return heap.peek();
    }

    public void pop() {
        if (heap.size() == 0) {
            throw new RuntimeException();
        }
        heap.poll();
    }

    public List<T> toList() {
        arr = new ArrayList<T>(heap);
        Collections.sort(arr, Collections.reverseOrder());
        return arr;
    }

    @Override
    public Iterator<T> iterator() {
        this.toList();
        return arr.iterator();
    }

    private Queue<T> heap;
    private List<T> arr;
    private int maxSize = 1;
    private Comparator<T> tComparator;
    private boolean hasComparator = false;
}

public class Main {
    public static void main(String[] args) {
        Test_LimitedHeap<Integer> test = new Test_LimitedHeap<>();

        //test 1
        List<Integer> stream1 = new LinkedList<>(Arrays.asList(2, 7, 6, 5, 1, 3, 9));
        List<Integer> ans1 = new LinkedList<>(Arrays.asList(5, 3, 2, 1));
        test.expectSuccess(stream1, 4, ans1);

        //test 2
        List<Integer> stream2 = new LinkedList<>(Arrays.asList(1, 1, 1, 1));
        List<Integer> ans2 = new LinkedList<>(Arrays.asList(1, 1));
        test.expectSuccess(stream2, 2, ans2);

        //test 3
        List<Integer> stream3 = new LinkedList<>(Arrays.asList(12, 7, 6, 4, 3, 2));
        List<Integer> ans3 = new LinkedList<>(Arrays.asList(12, 7, 6, 4, 3, 2));
        test.expectSuccess(stream3, 6, ans3);

        test.reportSummary();
    }
}
