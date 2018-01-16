package slidenerd.vivz.gpdemo;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.widget.LinearLayout;
import android.widget.TextView;

import java.util.ArrayList;

public class FirstActivity extends AppCompatActivity {

    LinearLayout parent,parent2;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_first);

        Log.d("s","asd");
        init();
        addTextViewToScrollView(40);
        addTextViewToScrollView2(40);
        setContent("丹丹漢堡");
    }

    public void init(){
        parent = (LinearLayout) findViewById(R.id.parent);
        parent2 = (LinearLayout) findViewById(R.id.parent2);
    }

    public void goToMap( View view ){
        Intent intent = new Intent( FirstActivity.this, MainActivity.class );
        startActivity(intent);
    }

    public void setContent( String s ){

    }

    public void addTextViewToScrollView( int size ){

        LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT,70);

        for( int i=0 ; i<size ; ++i ){

            TextView tx = new TextView(this);
            tx.setText(i+"");
            tx.setLayoutParams(lp);
            tx.setGravity(Gravity.START);
            tx.setTextSize(16);
            parent.addView(tx);
        }

    }


    public void addTextViewToScrollView2( int size ){
        LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT,70);

        for( int i=0 ; i<size ; ++i ){

            TextView tx = new TextView(this);
            tx.setText(i+"");
            tx.setLayoutParams(lp);
            tx.setGravity(Gravity.START);
            tx.setTextSize(16);
            parent2.addView(tx);
        }
    }
}
