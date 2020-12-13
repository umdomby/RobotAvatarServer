package by.umdom.udpserver2;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.net.wifi.WifiManager;
import android.os.IBinder;
import android.speech.tts.TextToSpeech;
import android.util.Log;
import org.greenrobot.eventbus.EventBus;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;
import java.util.Locale;
import java.util.Timer;
import java.util.TimerTask;

import by.umdom.udpserver2.eventbus.Event_MyService_UDPGet;

public class MyService extends Service {
    public MyService() {
    }

    //ByteArrayOutputStream outputByte = new ByteArrayOutputStream(12);
    StringBuilder stringBuilder = new StringBuilder();
    String[] items;
    private final static String TAG = MyService.class.getSimpleName();
    static final int UdpServerPORT = 7878;
    MyService.UdpServerThread udpServerThread;
    final String LOG_TAG = "myLogs";
    private StringBuilder sb = new StringBuilder(); //отправка данных UDP WiFi
    private TextToSpeech ttx;
    WifiManager wifiManager;
    DatagramPacket packet;
    Timer timer1;
    long heartbeatMillis;

    public void onCreate() {
        super.onCreate();

        Log.d(LOG_TAG, "onCreate");
    }

    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.d(LOG_TAG, "MyService onStartCommand, name = " + intent.getStringExtra("name"));
        readFlags(flags);
        udpServerThread = new MyService.UdpServerThread(UdpServerPORT);
        udpServerThread.start();
        wifiManager = (WifiManager) this.getSystemService(Context.WIFI_SERVICE);
        Log.d(LOG_TAG, "onStartCommand");
        getIpAddress();
        temerHardbit1();

        ttx =new TextToSpeech(getApplicationContext(), new TextToSpeech.OnInitListener() {
            @Override
            public void onInit(int status) {
                if(status != TextToSpeech.ERROR) {
                    Locale locale = new Locale("ru");
                    ttx.setLanguage(locale);
                }
            }
        });

        //return START_STICKY;
        return MyService.START_STICKY;
    }


    private void temerHardbit1() {
        timer1 = new Timer();
        timer1.scheduleAtFixedRate(new TimerTask() {
            @Override
            public void run() {
                //System.out.println(System.currentTimeMillis() +"   "+ heartbeatMillis);
                if( System.currentTimeMillis() > heartbeatMillis + 2000){
                //byte[] bufNull = new byte[256];
               // EventBus.getDefault().post(new Event_MyService_UDPGet(bufNull));
                }
            }
        }, 0, 100);
    }


    void readFlags(int flags){
        if ((flags&START_FLAG_REDELIVERY) == START_FLAG_REDELIVERY)
            Log.d(LOG_TAG, "START_FLAG REDELIVERY");
        if ((flags&START_FLAG_RETRY) == START_FLAG_RETRY)
            Log.d(LOG_TAG, "START_FLAG_RETRY");
    }

//приём данных UDP
    private class UdpServerThread extends Thread{
        int serverPort;
        DatagramSocket socket;
        boolean running;
        public UdpServerThread(int serverPort) {
            super();
            this.serverPort = serverPort;
        }
        public void setRunning(boolean running){ this.running = running;}

        @Override
        public void run() {
            running = true;
            try {
                socket = new DatagramSocket(serverPort);
                Log.e(TAG, "UDP Server is running");

                while(running){
                    byte[] buf = new byte[256];

                    packet = new DatagramPacket(buf, buf.length);
                    socket.receive(packet);

                    String str = new String(packet.getData(), packet.getOffset(), packet.getLength());
                    //outputByte.write(str.getBytes());
                    //byte joy_usb[] = outputByte.toByteArray();
                    //outputByte.reset();


                    stringBuilder.append(str);
                    System.out.print(str);

                    items = stringBuilder.toString().split(",");


//                    char c[] = new char[64];
//                    for(int i = 0 ; i < items.length; i++) {
//                        c[i] = items[i].charAt(0);
////                        System.out.print(items[i]);
////                        System.out.print("  ");
//                    }
//                    byte[] set_bytes = toBytes(c);
//

                    stringBuilder.delete(0, stringBuilder.length());
                    stringBuilder.setLength(0);

                    //String aaaaa = "1111";
                    //int bbb = Integer.parseInt(aaaaa);
                    //System.out.print("11111");



                    //outputByte.write(str.getBytes());

//                    String data = new String(buf, "UTF-8");
//                    System.out.print(data);
//                    System.out.print(buf);

//                    for(int i = 0 ; i < buf.length; i++) {
//                        System.out.print((char)buf[i] + " ");
//                    }

                    EventBus.getDefault().post(new Event_MyService_UDPGet(items));

//                    InetAddress address = packet.getAddress();
//                    int port = packet.getPort();
//                    packet = new DatagramPacket(buf, buf.length, address, port);
//                    socket.send(packet);

                    //seconds = ((System.currentTimeMillis() - startTime)) / 1000;

                    heartbeatMillis = System.currentTimeMillis();

                    //DatagramSocket clientSocket = new DatagramSocket();
                    InetAddress IPAddress = InetAddress.getByName("224.224.224.1");
                    //DatagramPacket sendPacket2 = new DatagramPacket(buf, buf.length, IPAddress, 7878);
                    //socket.send(sendPacket2);
                    //wifiManager.setWifiEnabled(false);
                    packet = new DatagramPacket(buf, buf.length, IPAddress, 7878);
                    socket.send(packet);
                    //wifiManager.setWifiEnabled(true);


//                    if (hartBit2 = false){
//                        temerHardbit2();
//                    }


//                    packet = new DatagramPacket(buf, buf.length, address, port);
//                    wifiManager.setWifiEnabled(false);
//                    socket.send(packet);
//                    wifiManager.setWifiEnabled(true);


//                    DatagramPacket sendPacket1 = new DatagramPacket(buf, buf.length, address, 7878);
//                    wifiManager.setWifiEnabled(false);
//                    clientSocket.send(sendPacket1);
//                    wifiManager.setWifiEnabled(true);

                    //String modifiedSentence = new String(packet.getData());
                    //int port = packet.getPort();
                    //String str = new String(packet.getData(), packet.getOffset(), packet.getLength());

                    //EventBus.getDefault().postSticky(new Event_MyService_UDPGet(str));
                    //send UDP WiFi
                    //String v = new String( buf, StandardCharsets.UTF_8 );
                    //if (buf[0] == 55){
                        //ttx.speak(v, TextToSpeech.QUEUE_FLUSH, null);
                    //}

//                    String dString = new Date().toString() + "\n"
//                            + "111111";
//                    buf = dString.getBytes();
//                    buf[1] = 2;
//                    packet = new DatagramPacket(buf, buf.length, address, port);
//                    wifiManager.setWifiEnabled(false);
//                    socket.send(packet);
//                    wifiManager.setWifiEnabled(true);


//                    DatagramPacket sendPacket1 = new DatagramPacket(buf, buf.length, address2, 7878);
//                    clientSocket.send(sendPacket1);
//


                }
                //Log.e(TAG, "UDP Server ended");

            } catch (SocketException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            } finally {
                if(socket != null){
                    socket.close();
                    //Log.e(TAG, "socket.close()");
                }
            }
        }
    }

//    byte[] toBytes(char[] chars) {
//        CharBuffer charBuffer = CharBuffer.wrap(chars);
//        ByteBuffer byteBuffer = Charset.forName("UTF-8").encode(charBuffer);
//        byte[] bytes = Arrays.copyOfRange(byteBuffer.array(),
//                byteBuffer.position(), byteBuffer.limit());
//        Arrays.fill(byteBuffer.array(), (byte) 0); // clear sensitive data
//        return bytes;
//    }

    private String getIpAddress() {
        String ip = "";
        try {
            Enumeration<NetworkInterface> enumNetworkInterfaces = NetworkInterface
                    .getNetworkInterfaces();
            while (enumNetworkInterfaces.hasMoreElements()) {
                NetworkInterface networkInterface = enumNetworkInterfaces
                        .nextElement();
                Enumeration<InetAddress> enumInetAddress = networkInterface
                        .getInetAddresses();
                while (enumInetAddress.hasMoreElements()) {
                    InetAddress inetAddress = enumInetAddress.nextElement();

                    if (inetAddress.isSiteLocalAddress()) {
                        ip += "SiteLocalAddress: "
                                + inetAddress.getHostAddress() + "\n";
                    }
                }
            }

        } catch (SocketException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
            ip += "Something Wrong! " + e.toString() + "\n";
        }
        return ip;
    }

    public IBinder onBind(Intent intent) {
        Log.d(LOG_TAG, "onBind");
        return null;
    }

    public void onDestroy() {
        super.onDestroy();
        if(udpServerThread != null){
            udpServerThread.setRunning(false);
            udpServerThread = null;
        }
    }
}
