package slidenerd.vivz.gpdemo.rest;

import java.util.Map;

import retrofit.Callback;
import retrofit.http.GET;
import retrofit.http.QueryMap;
import slidenerd.vivz.gpdemo.model.ReturnDistanceMatrixAPI;

/**
 * Created by Antony Teng on 2018/1/16.
 */

public interface GoogleDistanceMatrixService {
    @GET("/maps/api/distancematrix/json")
    void getWalkTime(@QueryMap Map<String, String> options, Callback<ReturnDistanceMatrixAPI> callback);
}
