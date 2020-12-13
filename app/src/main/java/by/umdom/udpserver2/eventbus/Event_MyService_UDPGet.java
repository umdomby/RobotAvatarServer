package by.umdom.udpserver2.eventbus;

public class Event_MyService_UDPGet extends DataEvent<String[]> {
    public Event_MyService_UDPGet(String[] text) {
        super(text);
    }
}