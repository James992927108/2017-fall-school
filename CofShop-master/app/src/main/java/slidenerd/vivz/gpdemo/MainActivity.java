package slidenerd.vivz.gpdemo;

import android.location.Location;
import android.os.Bundle;
import android.support.v7.widget.GridLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.widget.Toast;

import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.model.BitmapDescriptorFactory;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.Marker;
import com.google.android.gms.maps.model.MarkerOptions;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import retrofit.Callback;
import retrofit.RestAdapter;
import retrofit.RetrofitError;
import retrofit.client.Response;
import slidenerd.vivz.gpdemo.log.L;
import slidenerd.vivz.gpdemo.model.RestaurantShops;
import slidenerd.vivz.gpdemo.model.Results;
import slidenerd.vivz.gpdemo.rest.GooglePlacesService;

//-19.057045,72.905668

public class MainActivity extends BaseActivity implements OnMapReadyCallback, GoogleMap.OnMarkerClickListener {

    private GoogleMap mMap;
    private Toolbar mToolbar;
    private RecyclerView mRecyclerRestaurantShops;
    private RestaurantShopsAdapter mAdapter;

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
        initMap();
        initActionBar();
        initRecycler();
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
        markMyLocation(latLng);
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
        }
        return false;
    }
   
    public class RestaurantShopsCallback implements Callback<RestaurantShops> {
        @Override
        public void success(RestaurantShops coffeeShops, Response response) {
            Log.d("VIVZ", coffeeShops.toString());
            String status = coffeeShops.getStatus();

            if (status.equals(getString(R.string.status_ok))) {

                ArrayList<Results> listRestaurantShops = new ArrayList<>(40);
                //Normal flow of events
                for (Results current : coffeeShops.getResults()) {
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
}
