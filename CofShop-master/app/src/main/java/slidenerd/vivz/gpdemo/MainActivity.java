package slidenerd.vivz.gpdemo;

import android.content.Intent;
import android.location.Location;
import android.net.Uri;
import android.os.Bundle;
import android.support.v7.widget.GridLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.model.BitmapDescriptorFactory;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.Marker;
import com.google.android.gms.maps.model.MarkerOptions;

import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import io.realm.Realm;
import io.realm.RealmResults;
import retrofit.Callback;
import retrofit.RestAdapter;
import retrofit.RetrofitError;
import retrofit.client.Response;
import slidenerd.vivz.gpdemo.log.L;
import slidenerd.vivz.gpdemo.model.RestaurantDB;
import slidenerd.vivz.gpdemo.model.RestaurantShops;
import slidenerd.vivz.gpdemo.model.Results;
import slidenerd.vivz.gpdemo.rest.GooglePlacesService;

//-19.057045,72.905668

public class MainActivity extends BaseActivity implements OnMapReadyCallback, GoogleMap.OnMarkerClickListener {

    private GoogleMap mMap;
    private Toolbar mToolbar;
    private RecyclerView mRecyclerRestaurantShops;
    private RestaurantShopsAdapter mAdapter;

    private Realm realm;
    private TextView toolbar_textView;
    private void loadNearbyRestaurantShops(double latitude, double longitude) {
        RestAdapter restAdapter = new RestAdapter.Builder()
                .setEndpoint("https://maps.googleapis.com")
                .setLogLevel(RestAdapter.LogLevel.BASIC)
                .setLog(new RestAdapter.Log() {

                    @Override
                    public void log(String message) {
                        Log.d("VIVZ", message);
                    }
                }).build();

        GooglePlacesService service = restAdapter.create(GooglePlacesService.class);
        service.getCafes(getHashMapWithQueryParameters(latitude, longitude), new RestaurantShopsCallback());
    }

    private Map<String, String> getHashMapWithQueryParameters(double latitude, double longitude) {
        Map<String, String> map = new HashMap<>(5);
        map.put("sensor", true + "");
        map.put("types", "restaurant");
        map.put("radius", "500");
        map.put("location", getCsvLatLng(latitude, longitude));
        map.put("key", getString(R.string.google_places_api_key));
        return map;
    }

    private String getCsvLatLng(double latitude, double longitude) {
        return latitude + "," + longitude;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        realm = Realm.getDefaultInstance(); // opens "myrealm.realm"
        toolbar_textView = (TextView)findViewById(R.id.toolbar_textView);
        initMap();
        initActionBar();
        initRecycler();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        realm.close();
    }

    private void initActionBar() {
        mToolbar = (Toolbar) findViewById(R.id.app_bar);
        setSupportActionBar(mToolbar);
    }

    private void initMap() {
        SupportMapFragment mapFragment = (SupportMapFragment) getSupportFragmentManager().findFragmentById(R.id.map);
        mapFragment.getMapAsync(this);
    }

    private void initRecycler() {
        mAdapter = new RestaurantShopsAdapter(this);
        mRecyclerRestaurantShops = (RecyclerView) findViewById(R.id.recycler_coffee_shops);
        mRecyclerRestaurantShops.setLayoutManager(new GridLayoutManager(this, 1, GridLayoutManager.HORIZONTAL, false));
        mRecyclerRestaurantShops.setAdapter(mAdapter);
    }

    @Override
    public int getContentView() {
        return R.layout.activity_main;
    }

    @Override
    public void onLocationUpdate(LatLng latLng) {
        Log.d("Test", "onLocationUpdate");
        markMyLocation(latLng);// add my location marker
        loadNearbyRestaurantShops(latLng.latitude, latLng.longitude);
    }
    public void markMyLocation(LatLng latLng)
    {
        mMap.addMarker(new MarkerOptions()
                .position(new LatLng(latLng.latitude, latLng.longitude))
                .title("Current Position")
        );
    }
    @Override
    public void onMapReady(GoogleMap googleMap) {
        Log.d("Test", "onMapReady");
        mMap = googleMap;
        //mMap.addMarker(new MarkerOptions().position(new LatLng(0, 0)));
        mMap.animateCamera(CameraUpdateFactory.newLatLngZoom(new LatLng(0, 0), 20.0F));
        mMap.setOnMarkerClickListener(this);
    }

    @Override
    public boolean onMarkerClick(Marker marker) {
        int position = mAdapter.getPosition(marker.getTitle());
        if (position != -1) {
            mRecyclerRestaurantShops.smoothScrollToPosition(position);
            refresh_DB();
            export_DB();
        }
        return false;
    }
   
    public class RestaurantShopsCallback implements Callback<RestaurantShops> {
        @Override
        public void success(RestaurantShops restaurantShops, Response response) {
            Log.d("VIVZ", restaurantShops.toString());
            String status = restaurantShops.getStatus();

            if (status.equals(getString(R.string.status_ok))) {
                ArrayList<Results> listRestaurantShops = new ArrayList<>(40);
                //Normal flow of events
                for (Results current : restaurantShops.getResults()) {
                    save_to_DB(current.getName());// save result to datebase
                    double latitude = Double.valueOf(current.getGeometry().getLocation().getLatitude());
                    double longitude = Double.valueOf(current.getGeometry().getLocation().getLongitude());
                    LatLng position = new LatLng(latitude, longitude);
                    MarkerOptions markerOptions = new MarkerOptions();
                    markerOptions.position(position)
                            .title(current.getName())
                            .icon(BitmapDescriptorFactory.fromResource(R.drawable.ic_coficon));//加入圖標
                    mMap.addMarker(markerOptions);
                    mMap.moveCamera(CameraUpdateFactory.newLatLng(position));
                    listRestaurantShops.add(current);
                }

                mAdapter.setDataSource(listRestaurantShops);
            } else if (status.equals(getString(R.string.status_over_query_limit))) {
                //Do actions to indicate the developer that the tier for this application must be increased
            } else if (status.equals(getString(R.string.status_request_denied))) {
                //The key is invalid in this case
                Toast.makeText(MainActivity.this, "Request Denied", Toast.LENGTH_SHORT).show();
            } else if (status.equals(getString(R.string.status_invalid_request))) {
                //Some parameters are missing
            } else {
                L.s(MainActivity.this, "Are you stranded on an island? Because we didnt find any coffee shops near you");
            }
        }

        @Override
        public void failure(RetrofitError error) {
            L.s(MainActivity.this, error.toString());
        }
    }
    private void save_to_DB(final String restaurantShops)
    {
        RestaurantDB shop = new RestaurantDB();
        shop.setName(restaurantShops);

        realm.beginTransaction();
        RestaurantDB realmUser = realm.copyToRealmOrUpdate(shop);
        realm.commitTransaction();

//        realm.executeTransactionAsync(new Realm.Transaction() {
//            @Override
//            public void execute(Realm bgRealm) {
//                RestaurantDB shop = bgRealm.createObject(RestaurantDB.class);
//                shop.setName(restaurantShops);
//            }
//        }, new Realm.Transaction.OnSuccess() {
//            @Override
//            public void onSuccess() {
//                Log.d("Success", "Save to DB Success");
//            }
//        }, new Realm.Transaction.OnError() {
//            @Override
//            public void onError(Throwable error) {
//                Log.d("Error", error.getMessage());
//            }
//        });
    }
    private void ForTestDBinsert()
    {
        RestaurantDB shop = new RestaurantDB();
        shop.setName("");
// Copy the object to Realm. Any further changes must happen on realmUser
        realm.beginTransaction();
        RestaurantDB realmUser = realm.copyToRealm(shop);
        realm.commitTransaction();
    }
    private void refresh_DB()
    {
        RealmResults<RestaurantDB> result = realm.where(RestaurantDB.class).findAllAsync();
        result.load();
//        String Output ="";
//        for (RestaurantDB shop:result) {
//            Output += shop.toString();
//        }
//        toolbar_textView.setText(Output);
    }
    public void export_DB() {
        // init realm
        Realm realm = Realm.getDefaultInstance();
        File exportRealmFile = null;
        // get or create an "export.realm" file
        exportRealmFile = new File(this.getExternalCacheDir(), "export.realm");
        // if "export.realm" already exists, delete
        exportRealmFile.delete();
        // copy current realm to "export.realm"
        realm.writeCopyTo(exportRealmFile);
        realm.close();
        // init email intent and add export.realm as attachment
        Intent intent = new Intent(Intent.ACTION_SEND);
        intent.setType("plain/text");
        intent.putExtra(Intent.EXTRA_EMAIL, "james992927108@gmail.com");
        intent.putExtra(Intent.EXTRA_SUBJECT, "TestTest");
        intent.putExtra(Intent.EXTRA_TEXT, "realm test");
        Uri u = Uri.fromFile(exportRealmFile);
        intent.putExtra(Intent.EXTRA_STREAM, u);

        // start email intent
        startActivity(Intent.createChooser(intent, "123"));
    }
}
