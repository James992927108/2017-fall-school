package slidenerd.vivz.gpdemo.model.DistanceMatrixApi;

import com.google.gson.annotations.SerializedName;

import java.util.Arrays;

/**
 * Created by Antony Teng on 2018/1/16.
 */

public class ReturnDistanceMatrixAPI {

    @SerializedName("destination_addresses")
    private String[] destinationAddresses;

    @SerializedName("origin_addresses")
    private String[] originAddresses;

    @SerializedName("rows")
    private Rows[] rows;

    @SerializedName("status")
    private String status;

    @Override
    public String toString() {
        return "ReturnDistanceMatrixAPI{" +
                "destinationAddresses=" + Arrays.toString(destinationAddresses) +
                ", originAddresses=" + Arrays.toString(originAddresses) +
                ", rows=" + Arrays.toString(rows) +
                ", status='" + status + '\'' +
                '}';
    }

    public String[] getDestinationAddresses() {
        return destinationAddresses;
    }

    public void setDestinationAddresses(String[] destinationAddresses) {
        this.destinationAddresses = destinationAddresses;
    }

    public String[] getOriginAddresses() {
        return originAddresses;
    }

    public void setOriginAddresses(String[] originAddresses) {
        this.originAddresses = originAddresses;
    }

    public Rows[] getRows() {
        return rows;
    }

    public void setRows(Rows[] rows) {
        this.rows = rows;
    }

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }
}
