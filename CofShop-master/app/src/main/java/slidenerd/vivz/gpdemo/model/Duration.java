package slidenerd.vivz.gpdemo.model;

/**
 * Created by Antony Teng on 2018/1/16.
 */

public class Duration {
    private int value;
    private String text;

    @Override
    public String toString() {
        return "Duration{" +
                "value=" + value +
                ", text='" + text + '\'' +
                '}';
    }

    public int getValue() {
        return value;
    }

    public void setValue(int value) {
        this.value = value;
    }

    public String getText() {
        return text;
    }

    public void setText(String text) {
        this.text = text;
    }
}
