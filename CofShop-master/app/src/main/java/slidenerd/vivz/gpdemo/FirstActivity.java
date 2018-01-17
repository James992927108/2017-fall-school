package slidenerd.vivz.gpdemo;

<<<<<<< HEAD
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Gravity;

import android.widget.LinearLayout;
import android.widget.TextView;

import io.realm.Realm;

import io.realm.RealmResults;
import io.realm.Sort;
import slidenerd.vivz.gpdemo.model.DB.RestaurantDB;

public class FirstActivity extends AppCompatActivity {

    LinearLayout parent, parent2;
    private Realm realm;
=======
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
>>>>>>> 073cf0981f561097b1b31800639eb4d0f528b0c6

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_first);

<<<<<<< HEAD
        init();
        initDB();
        refresh_DB();
        addTextViewToScrollView(40);
        addTextViewToScrollView2(40);

    }

    public void init() {
=======
        Log.d("s","asd");
        init();
        addTextViewToScrollView(40);
        addTextViewToScrollView2(40);
        setContent("丹丹漢堡");
    }

    public void init(){
>>>>>>> 073cf0981f561097b1b31800639eb4d0f528b0c6
        parent = (LinearLayout) findViewById(R.id.parent);
        parent2 = (LinearLayout) findViewById(R.id.parent2);
    }

<<<<<<< HEAD
    private void initDB() {
        realm = Realm.getDefaultInstance();
    }

    private void refresh_DB() {
        RealmResults<RestaurantDB> result = realm.where(RestaurantDB.class).findAllAsync();
        result.load();
    }

    public void addTextViewToScrollView(int size) {

        LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, 80);
        int lastUpdateCount = realm.where(RestaurantDB.class).sort("UpdateCount",Sort.DESCENDING).findAll().first().getUpdateCount();
        RealmResults<RestaurantDB> result1 = realm.where(RestaurantDB.class).beginGroup().equalTo("UpdateCount",lastUpdateCount).endGroup().findAll();
        for (int i = 0; i < result1.size(); ++i) {
            TextView tx = new TextView(this);
            tx.setText(i + " " + result1.get(i).getName());
=======
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
>>>>>>> 073cf0981f561097b1b31800639eb4d0f528b0c6
            tx.setLayoutParams(lp);
            tx.setGravity(Gravity.START);
            tx.setTextSize(16);
            parent.addView(tx);
        }
<<<<<<< HEAD
    }

    public void addTextViewToScrollView2(int size) {
        LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, 80);
        RealmResults<RestaurantDB> result1 = realm.where(RestaurantDB.class).sort("Rating", Sort.DESCENDING).findAll();
        for (int i = 0; i < result1.size(); ++i) {
            TextView tx = new TextView(this);
            tx.setText(i + " " + result1.get(i).getName());
=======

    }


    public void addTextViewToScrollView2( int size ){
        LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT,70);

        for( int i=0 ; i<size ; ++i ){

            TextView tx = new TextView(this);
            tx.setText(i+"");
>>>>>>> 073cf0981f561097b1b31800639eb4d0f528b0c6
            tx.setLayoutParams(lp);
            tx.setGravity(Gravity.START);
            tx.setTextSize(16);
            parent2.addView(tx);
        }
    }
}
