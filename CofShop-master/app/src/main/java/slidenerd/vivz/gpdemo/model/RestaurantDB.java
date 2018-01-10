package slidenerd.vivz.gpdemo.model;

import com.google.android.gms.maps.model.LatLng;

import io.realm.RealmObject;
import io.realm.annotations.PrimaryKey;

/**
 * Created by Antony Teng on 2018/1/3.
 */
public class RestaurantDB extends RealmObject {
    @PrimaryKey
    private String Name;

    private double latitude;
    private double longitude;
    private int VisitCount;

    @Override
    public String toString() {
        return "RestaurantDB{" +
                "Name='" + Name + '\'' +
                ", latitude=" + latitude +
                ", longitude=" + longitude +
                ", VisitCount=" + VisitCount +
                '}';
    }

    public String getName() {
        return Name;
    }

    public void setName(String name) {
        Name = name;
    }

    public double getLatitude() {
        return latitude;
    }

    public void setLatitude(double latitude) {
        this.latitude = latitude;
    }

    public double getLongitude() {
        return longitude;
    }

    public void setLongitude(double longitude) {
        this.longitude = longitude;
    }

    public int getVisitCount() {
        return VisitCount;
    }

    public void setVisitCount(int visitCount) {
        VisitCount = visitCount;
    }
}
