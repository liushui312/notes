
Iterator    
 迭代器一般和泛型数组ArrayList，泛型链表LinkList配合使用，达到顺序遍历的效果.

使用：
 1. 定义泛型数组
    ArrayList<对象类型> mlist = new ArrayList<对象类型>;

 2. 给mlist加上迭代器，对它进行顺序遍历操作 
    Iterator<对象类型> it = mlist.iterator();

 3. 开始遍历
    while(it.hasNext()) {
        it.next()
        ...
    }
