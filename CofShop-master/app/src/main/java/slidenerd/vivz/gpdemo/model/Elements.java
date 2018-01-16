package slidenerd.vivz.gpdemo.model;

/**
 * Created by Antony Teng on 2018/1/16.
 */

public class Elements {
    //(duration_in_traffic,fare) not use
    private String status;
    private Duration duration;
    private Duration distance;
    @Override
    public String toString() {
        return "Elements{" +
                "status='" + status + '\'' +
                ", duration=" + duration +
                ", distance=" + distance +
                '}';
    }

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }

    public Duration getDuration() {
        return duration;
    }

    public void setDuration(Duration duration) {
        this.duration = duration;
    }

    public Duration getDistance() {
        return distance;
    }

    public void setDistance(Duration distance) {
        this.distance = distance;
    }
}
