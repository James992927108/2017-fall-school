package slidenerd.vivz.gpdemo.model;

import io.realm.RealmObject;
import io.realm.annotations.PrimaryKey;

/**
 * Created by Antony Teng on 2018/1/3.
 */
public class RestaurantDB extends RealmObject {
    @PrimaryKey
    private String Name;
    private int VisitCount;

    public String getName() {
        return Name;
    }

    public void setName(String name) {
        Name = name;
    }

    public int getVisitCount() {
        return VisitCount;
    }

    public void setVisitCount(int visitCount) {
        VisitCount = visitCount;
    }

    @Override
    public String toString() {
        return "RestaurantDB{" +
                "Name='" + Name + '\'' +
                ", VisitCount=" + VisitCount +
                '}';
    }
}
