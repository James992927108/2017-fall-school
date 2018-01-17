package slidenerd.vivz.gpdemo.model.PlaceApi;

import com.google.gson.annotations.SerializedName;

import java.util.Arrays;

import slidenerd.vivz.gpdemo.model.PlaceApi.ResultModel.Geometry;
import slidenerd.vivz.gpdemo.model.PlaceApi.ResultModel.OpeningHours;
import slidenerd.vivz.gpdemo.model.PlaceApi.ResultModel.Photos;

public class Results {

    @SerializedName("icon")
    private String icon;

    @SerializedName("place_id")
    private String placeId;

    @SerializedName("scope")
    private String scope;

    @SerializedName("reference")
    private String reference;

    @SerializedName("geometry")
    private Geometry geometry;

    @SerializedName("opening_hours")
    private OpeningHours openingHours;

    @SerializedName("id")
    private String id;

    @SerializedName("photos")
    private Photos[] photos;

    @SerializedName("price_levels")
    private String priceLevel;

    @SerializedName("vicinity")
    private String vicinity;

    @SerializedName("name")
    private String name;

    @SerializedName("rating")
    private double rating;

    private String[] types;

    @Override
    public String toString() {
        return "Results{" +
                "icon='" + icon + '\'' +
                ", placeId='" + placeId + '\'' +
                ", scope='" + scope + '\'' +
                ", reference='" + reference + '\'' +
                ", geometry=" + geometry +
                ", openingHours=" + openingHours +
                ", id='" + id + '\'' +
                ", photos=" + Arrays.toString(photos) +
                ", priceLevel='" + priceLevel + '\'' +
                ", vicinity='" + vicinity + '\'' +
                ", name='" + name + '\'' +
                ", rating=" + rating +
                ", types=" + Arrays.toString(types) +
                '}';
    }

    public String getIcon() {
        return icon;
    }

    public void setIcon(String icon) {
        this.icon = icon;
    }

    public String getPlaceId() {
        return placeId;
    }

    public void setPlaceId(String placeId) {
        this.placeId = placeId;
    }

    public String getScope() {
        return scope;
    }

    public void setScope(String scope) {
        this.scope = scope;
    }

    public String getReference() {
        return reference;
    }

    public void setReference(String reference) {
        this.reference = reference;
    }

    public Geometry getGeometry() {
        return geometry;
    }

    public void setGeometry(Geometry geometry) {
        this.geometry = geometry;
    }

    public OpeningHours getOpeningHours() {
        return openingHours;
    }

    public void setOpeningHours(OpeningHours openingHours) {
        this.openingHours = openingHours;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public Photos[] getPhotos() {
        return photos;
    }

    public void setPhotos(Photos[] photos) {
        this.photos = photos;
    }

    public String getPriceLevel() {
        return priceLevel;
    }

    public void setPriceLevel(String priceLevel) {
        this.priceLevel = priceLevel;
    }

    public String getVicinity() {
        return vicinity;
    }

    public void setVicinity(String vicinity) {
        this.vicinity = vicinity;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public double getRating() {
        return rating;
    }

    public void setRating(double rating) {
        this.rating = rating;
    }

    public String[] getTypes() {
        return types;
    }

    public void setTypes(String[] types) {
        this.types = types;
    }
}