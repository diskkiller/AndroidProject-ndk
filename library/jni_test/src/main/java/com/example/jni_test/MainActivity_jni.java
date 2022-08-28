package com.example.jni_test;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import com.example.jni_test.databinding.ActivityMainBinding;


public class MainActivity_jni extends AppCompatActivity {

    // Used to load the 'myjni' library on application startup.
    static {
        //System.load(D:/XXX/myjni);//绝对路径加载动态链接库
        System.loadLibrary("myjni");//从库目录遍历层级目录，自动寻找--apk的lib目录so库文件
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Example of a call to a native method
        TextView tv = binding.sampleText;

        int[] ints = new int[]{1,2,3,4,5,6};//基本类型
        String[] strs = new String[]{"李小龙","李连杰","李元霸"};//对象类型

        //arrayFromJNI(99,"你好",ints,strs);

        creatObject();

        tv.setText(stringFromJNI());
    }

    public void testObject(){
        Student student = new Student();
        student.setName("史泰龙");
        student.setAge(28);
        putObjectFromJNI(student,"无影掌");
    }

    public void creatObject(){
        creatObjectFromJNI();
    }

    /**
     * A native method that is implemented by the 'myjni' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native void arrayFromJNI(int count,String textInfo,int[] ints,String[] strs);

    public native void putObjectFromJNI(Student student,String info);

    public native void creatObjectFromJNI();//创建 Student (java) 对象
}