package slidenerd.vivz.gpdemo;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.Parcelable;
import android.support.v7.widget.GridLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.View;
import android.widget.Button;
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
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import io.realm.Realm;
import io.realm.RealmResults;
import io.realm.Sort;
import retrofit.Callback;
import retrofit.RestAdapter;
import retrofit.RetrofitError;
import retrofit.client.Response;
import slidenerd.vivz.gpdemo.log.L;
import slidenerd.vivz.gpdemo.model.DistanceMatrixApi.RowsModel.Elements;
import slidenerd.vivz.gpdemo.model.DistanceMatrixApi.ReturnDistanceMatrixAPI;
import slidenerd.vivz.gpdemo.model.PlaceApi.ReturnPlaceAPI;
import slidenerd.vivz.gpdemo.model.DB.RestaurantDB;
import slidenerd.vivz.gpdemo.model.PlaceApi.Results;
import slidenerd.vivz.gpdemo.model.DistanceMatrixApi.Rows;
import slidenerd.vivz.gpdemo.rest.GoogleDistanceMatrixService;
import slidenerd.vivz.gpdemo.rest.GooglePlacesService;

public class MainActivity extends BaseActivity implements OnMapReadyCallback, GoogleMap.OnMarkerClickListener {

    private GoogleMap mMap;
    private Toolbar mToolbar;
    private RecyclerView mRecyclerRestaurantShops;
    private RestaurantShopsAdapter mAdapter;

    private LatLng CurrentPosition;
    private Realm realm;

    private TextView toolbar_textView;

    private Button toolbar_button;

    private void loadNearbyRestaurantShops(LatLng OriginPosition) {
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
        service.getCafes(getHashMapWithQueryParametersInPlaceService(OriginPosition), new RestaurantShopsCallback());
    }
    private Map<String, String> getHashMapWithQueryParametersInPlaceService(LatLng OriginPosition) {
        Map<String, String> map = new HashMap<>(5);
        //map.put("sensor", true + "");
        map.put("language","zh-TW");
        map.put("type", "restaurant");
        map.put("radius", "500");
        //map.put("rankby","distance");
        map.put("location", getCsvLatLng(OriginPosition.latitude, OriginPosition.longitude));
        map.put("key", getString(R.string.google_places_api_key));
        return map;
    }

    private void loadWalkTime(LatLng OriginPosition , LatLng DestinationPosition) {
        RestAdapter restAdapter = new RestAdapter.Builder()
                .setEndpoint("https://maps.googleapis.com")
                .setLogLevel(RestAdapter.LogLevel.BASIC)
                .setLog(new RestAdapter.Log() {
                    @Override
                    public void log(String message) {
                        Log.d("VIVZ", message);
                    }
                }).build();
        GoogleDistanceMatrixService service = restAdapter.create(GoogleDistanceMatrixService.class);
        service.getWalkTime(getHashMapWithQueryParametersInDistanceMatrixService(OriginPosition,DestinationPosition), new WalkTimeCallback());
    }

    private Map<String, String> getHashMapWithQueryParametersInDistanceMatrixService(LatLng OriginPosition,LatLng DestinationPosition) {
        Map<String, String> map = new HashMap<>(5);
        map.put("origins", getCsvLatLng(OriginPosition.latitude, OriginPosition.longitude));
        map.put("destinations", getCsvLatLng(DestinationPosition.latitude, DestinationPosition.longitude));
        map.put("mode ","walking ");
        map.put("language","zh-TW");
        map.put("key", getString(R.string.google_distance_matrix_api_key));
        return map;
    }

    private String getCsvLatLng(double latitude, double longitude) {
        return latitude + "," + longitude;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
<<<<<<< HEAD
        toolbar_textView = (TextView) findViewById(R.id.toolbar_textView);
=======
        realm = Realm.getDefaultInstance();
        toolbar_textView = (TextView)findViewById(R.id.toolbar_textView);

>>>>>>> 073cf0981f561097b1b31800639eb4d0f528b0c6
        toolbar_button = (Button) findViewById(R.id.bt_debug);
        toolbar_button.setOnClickListener(new Button.OnClickListener() {
            @Override
            public void onClick(View v) {
                refresh_DB();
                export_DB();
            }
        });
<<<<<<< HEAD
        initDB();
=======
>>>>>>> 073cf0981f561097b1b31800639eb4d0f528b0c6
        initMap();
        initActionBar();
        initRecycler();
    }

    public void goToMap(View view) {
        refresh_DB();
        Intent intent = new Intent(MainActivity.this, FirstActivity.class);
        startActivity(intent);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        realm.close();
    }

    private void initDB() {
        realm = Realm.getDefaultInstance();
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
    public void onLocationUpdate(LatLng OriginPosition) {
        Log.d("Test", "onLocationUpdate");
        CurrentPosition = OriginPosition;
        markMyLocation(OriginPosition);// add my location marker
        loadNearbyRestaurantShops(OriginPosition);
        checkArrivalDestination(OriginPosition);
    }
    public void markMyLocation(LatLng OriginPosition)
    {
        mMap.addMarker(new MarkerOptions()
                .position(new LatLng(OriginPosition.latitude, OriginPosition.longitude))
                .title("Current Position")
        );
<<<<<<< HEAD
        mMap.moveCamera(CameraUpdateFactory.newLatLngZoom(CurrentPosition, 15.0f));
    }

    public void checkArrivalDestination(LatLng OriginPosition) {//22.6239637,120.270525
        final LatLng LatLngFormat = DecimalFormat(OriginPosition.latitude, OriginPosition.longitude);
=======
        mMap.moveCamera(CameraUpdateFactory.newLatLngZoom(CurrentPosition,15.0f));
    }
    public void checkArrivalDestination(LatLng OriginPosition)
    {//22.6239637,120.270525
        final LatLng LatLngDecimalFormat = DecimalFormat(OriginPosition);
>>>>>>> 073cf0981f561097b1b31800639eb4d0f528b0c6
        realm.executeTransaction(new Realm.Transaction() {
            @Override
            public void execute(Realm realm) {
                RestaurantDB result = realm.where(RestaurantDB.class)
<<<<<<< HEAD
                        .equalTo("Latitude", LatLngFormat.latitude)
                        .and()
                        .equalTo("Longitude", LatLngFormat.longitude)
=======
                        .equalTo("latitude",LatLngDecimalFormat.latitude)
                        .and()
                        .equalTo("longitude",LatLngDecimalFormat.longitude)
>>>>>>> 073cf0981f561097b1b31800639eb4d0f528b0c6
                        .findFirst();
                if(result != null)
                {
                    int lastVisitCount = result.getVisitCount();
                    lastVisitCount += 1;
                    result.setVisitCount(lastVisitCount);
                }
            }
        });
//        RestaurantDB result1 = realm.where(RestaurantDB.class)
//                .equalTo("latitude",22.624)
//                .and()
//                .equalTo("longitude",120.2705)
//                .findFirst();
    }

    @Override
    public void onMapReady(GoogleMap googleMap) {
        Log.d("Test", "onMapReady");
        mMap = googleMap;
<<<<<<< HEAD
=======
        //mMap.addMarker(new MarkerOptions().position(new LatLng(0, 0)));
//        mMap.animateCamera(CameraUpdateFactory.newLatLngZoom(new LatLng(0, 0), 20.0F));
>>>>>>> 073cf0981f561097b1b31800639eb4d0f528b0c6
        mMap.moveCamera(CameraUpdateFactory.newLatLngZoom(new LatLng(0, 0), 20.0f));
        mMap.setOnMarkerClickListener(this);
    }

    @Override
    public boolean onMarkerClick(Marker marker) {
        int positionIndex = mAdapter.getPositionIndex(marker.getTitle());
        LatLng DestinationPosition = mAdapter.getPosition(marker.getTitle());
        LatLng OriginPosition = CurrentPosition ;
        if (positionIndex != -1) {
            mRecyclerRestaurantShops.smoothScrollToPosition(positionIndex);
            loadWalkTime(OriginPosition,DestinationPosition);
        }
        return false;
    }
    public class RestaurantShopsCallback implements Callback<ReturnPlaceAPI> {
        @Override
        public void success(ReturnPlaceAPI returnPlaceAPI, Response response) {
            Log.d("RestaurantShopsCallback", returnPlaceAPI.toString());
            String status = returnPlaceAPI.getStatus();
            if (status.equals(getString(R.string.status_ok))) {
                ArrayList<Results> listRestaurantShops = new ArrayList<>(40);
                ArrayList<RestaurantDB> listDB = new ArrayList<>(40);

                //Normal flow of events
                for (Results current : returnPlaceAPI.getResults()) {
                    double latitude = Double.valueOf(current.getGeometry().getLocation().getLatitude());
                    double longitude = Double.valueOf(current.getGeometry().getLocation().getLongitude());
                    LatLng position = new LatLng(latitude, longitude);
<<<<<<< HEAD

                    RestaurantDB value = new RestaurantDB(current.getName(), latitude, longitude, current.getRating(), 0, 0);
                    listDB.add(value);
=======
                    save_to_DB(current.getName(),position);// save result to DB
>>>>>>> 073cf0981f561097b1b31800639eb4d0f528b0c6

                    MarkerOptions markerOptions = new MarkerOptions();
                    markerOptions.position(position)
                            .title(current.getName())
                            .icon(BitmapDescriptorFactory.fromResource(R.drawable.ic_coficon));//加入圖標
                    mMap.addMarker(markerOptions);
                    listRestaurantShops.add(current);
                }
                save_to_DB(listDB);// save result to DB
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

    public class WalkTimeCallback implements Callback<ReturnDistanceMatrixAPI> {
        @Override
        public void success(ReturnDistanceMatrixAPI returnDistanceMatrixAPI, Response response) {
            Log.d("WalkTimeCallback", returnDistanceMatrixAPI.toString());
            String status = returnDistanceMatrixAPI.getStatus();
            if (status.equals(getString(R.string.status_ok))) {
                //設定 Text
<<<<<<< HEAD
                for (Rows current : returnDistanceMatrixAPI.getRows()) {
                    for (Elements item : current.getElements()) {
                        Log.d("returnDistanceMatrixAPI", item.getDuration().getText());
=======
                for(Rows current : returnDistanceMatrixAPI.getRows())
                {
                    for(Elements item: current.getElements())
                    {
                        Log.d("returnDistanceMatrixAPI",item.getDuration().getText());
>>>>>>> 073cf0981f561097b1b31800639eb4d0f528b0c6
                        toolbar_textView.setText(item.getDuration().getText());
                    }
                }
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
            Log.d("WalkTimeCallbackERROR",  error.toString());
            L.s(MainActivity.this, error.toString());
        }
    }

<<<<<<< HEAD
    private void save_to_DB(ArrayList<RestaurantDB> value) {
        int lastUpdateCount = 0;
        RestaurantDB result = realm.where(RestaurantDB.class).sort("UpdateCount", Sort.DESCENDING).findAll().first();
        if(result != null)
        {
            lastUpdateCount = result.getUpdateCount();
            lastUpdateCount += 1;
        }
        for(RestaurantDB current : value)
        {
            LatLng LatLngFormat = DecimalFormat(current.getLatitude(), current.getLongitude());
            RestaurantDB shop = new RestaurantDB(current.getName(), LatLngFormat.latitude, LatLngFormat.longitude, current.getRating(), current.getVisitCount(), lastUpdateCount);

            realm.beginTransaction();
            RestaurantDB realmUser = realm.copyToRealmOrUpdate(shop);//only update the date which not in DB
            realm.commitTransaction();
        }

    }

    private LatLng DecimalFormat(double latitude, double longitude) {
        DecimalFormat df = new DecimalFormat("##.0000");
        LatLng LatLngDecimalFormat = new LatLng(Double.parseDouble(df.format(latitude)), Double.parseDouble(df.format(longitude)));
=======
    private void save_to_DB(final String name,LatLng position)
    {
        LatLng LatLngDecimalFormat = DecimalFormat(position);
        RestaurantDB shop = new RestaurantDB();
        shop.setName(name);
        shop.setLatitude(LatLngDecimalFormat.latitude);
        shop.setLongitude(LatLngDecimalFormat.longitude);
        realm.beginTransaction();
        RestaurantDB realmUser = realm.copyToRealmOrUpdate(shop);//only update the date which not in DB
        realm.commitTransaction();
    }
    private LatLng DecimalFormat(LatLng position)
    {
        double Latitude = position.latitude;
        double Longitude = position.longitude;
        DecimalFormat df = new DecimalFormat("##.0000");
        Latitude = Double.parseDouble(df.format(Latitude));
        Longitude = Double.parseDouble(df.format(Longitude));
        LatLng LatLngDecimalFormat = new LatLng(Latitude,Longitude);
>>>>>>> 073cf0981f561097b1b31800639eb4d0f528b0c6
        return LatLngDecimalFormat;
    }

    private void refresh_DB()
    {
        RealmResults<RestaurantDB> result = realm.where(RestaurantDB.class).findAllAsync();
        result.load();
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
