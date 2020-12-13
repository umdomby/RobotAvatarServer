package by.umdom.udpserver2.eventbus;

public class DataEvent<T> {

    private T data;

    public DataEvent(T data) {

        this.data = data;
    }

    public T getData() {

        return data;
    }
}
