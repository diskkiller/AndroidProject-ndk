package com.example.jni_test;

import android.util.Log;

public class Student {
    public String name;
    public int age;

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
        Log.d("Student setName：",name);
    }

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        this.age = age;
        Log.d("Student setAge：",age+"");
    }
    public static void showInfo(String info){
        Log.d("Student","showInfo: "+info);
    }

    @Override
    public String toString() {
        return "Student{" +
                "name='" + name + '\'' +
                ", age=" + age +
                '}';
    }
}
