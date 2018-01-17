package slidenerd.vivz.gpdemo.model.DB;

import com.google.android.gms.maps.model.LatLng;

import io.realm.RealmObject;
import io.realm.annotations.PrimaryKey;

/**
 * Created by Antony Teng on 2018/1/3.
 */
public class RestaurantDB extends RealmObject {

    @PrimaryKey
    private String Name;
    private double Latitude;
    private double Longitude;
    private double Rating;
    private int VisitCount;
    private int UpdateCount;

    @Override
    public String toString() {
        return "RestaurantDB{" +
                "Name='" + Name + '\'' +
                ", Latitude=" + Latitude +
                ", Longitude=" + Longitude +
                ", Rating=" + Rating +
                ", VisitCount=" + VisitCount +
                ", UpdateCount=" + UpdateCount +
                '}';
    }



    public RestaurantDB(String name, double latitude, double longitude, double rating, int visitCount, int updateCount) {
        Name = name;
        Latitude = latitude;
        Longitude = longitude;
        Rating = rating;
        VisitCount = visitCount;
        UpdateCount = updateCount;
    }

    public String getName() {
        return Name;
    }

    public void setName(String name) {
        Name = name;
    }

    public double getLatitude() {
        return Latitude;
    }

    public void setLatitude(double latitude) {
        Latitude = latitude;
    }

    public double getLongitude() {
        return Longitude;
    }

    public void setLongitude(double longitude) {
        Longitude = longitude;
    }

    public double getRating() {
        return Rating;
    }

    public void setRating(double rating) {
        Rating = rating;
    }

    public int getVisitCount() {
        return VisitCount;
    }

    public void setVisitCount(int visitCount) {
        VisitCount = visitCount;
    }

    public int getUpdateCount() {
        return UpdateCount;
    }

    public void setUpdateCount(int updateCount) {
        UpdateCount = updateCount;
    }



    public RestaurantDB() {
    }

}
