package slidenerd.vivz.gpdemo.model.DistanceMatrixApi;

import com.google.gson.annotations.SerializedName;

import java.util.Arrays;

import slidenerd.vivz.gpdemo.model.DistanceMatrixApi.RowsModel.Elements;

/**
 * Created by Antony Teng on 2018/1/16.
 */

public class Rows {
    @SerializedName("elements")
    private Elements[] elements;

    @Override
    public String toString() {
        return "Rows{" +
                "elements=" + Arrays.toString(elements) +
                '}';
    }

    public Elements[] getElements() {
        return elements;
    }

    public void setElements(Elements[] elements) {
        this.elements = elements;
    }
}
