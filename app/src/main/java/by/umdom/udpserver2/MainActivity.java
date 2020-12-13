package by.umdom.udpserver2;

import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.speech.tts.TextToSpeech;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.TextView;

import org.greenrobot.eventbus.EventBus;
import org.greenrobot.eventbus.Subscribe;
import org.greenrobot.eventbus.ThreadMode;

import java.util.Locale;

import by.umdom.udpserver2.eventbus.Event_MyService_UDPGet;

public class MainActivity extends AppCompatActivity {

    private final static String TAG = MainActivity.class.getSimpleName();
    private TextView textjoy0,textjoy1,textjoy2,textjoy3,textjoy4,textjoy5,textjoy6,textjoy7,textjoy8, textlong;
    TextView infoIp, infoPort;
    TextView textViewState, textViewPrompt, txt_data;
    private TextToSpeech ttx;
    String SpeechTTX;
    String SpeechEnable;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getSupportActionBar().hide();
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        setContentView(R.layout.activity_main);
        EventBus.getDefault().register(this);

        infoIp = (TextView) findViewById(R.id.infoip);
        infoPort = (TextView) findViewById(R.id.infoport);
        textViewState = (TextView)findViewById(R.id.state);
        textViewPrompt = (TextView)findViewById(R.id.prompt);
        txt_data = (TextView)findViewById(R.id.txt_data);

        textjoy1 = findViewById(R.id.textjoy1);textjoy2 = findViewById(R.id.textjoy2);textjoy3 = findViewById(R.id.textjoy3);textjoy4 = findViewById(R.id.textjoy4);
        textjoy5 = findViewById(R.id.textjoy5);textjoy6 = findViewById(R.id.textjoy6);textjoy7 = findViewById(R.id.textjoy7);textjoy8 = findViewById(R.id.textjoy8);
        textjoy8 = findViewById(R.id.textjoy8);textjoy0 = findViewById(R.id.textjoy0);

        ttx =new TextToSpeech(getApplicationContext(), new TextToSpeech.OnInitListener() {
            @Override
            public void onInit(int status) {
                if(status != TextToSpeech.ERROR) {
                    Locale locale = new Locale("ru");
                    ttx.setLanguage(locale);
                }
            }
        });

        startService(new Intent(this, MyService.class));

//        infoIp.setText(getIpAddress());
//        infoPort.setText(String.valueOf(UdpServerPORT));
    }

    public void onClickStart(View v){
        startService(new Intent(this, MyService.class));

    }

    public void onClickStop(View v) {
        stopService(new Intent(this, MyService.class));
    }

    public void speak(View v) {
        //ttx.speak("Сергей привет", TextToSpeech.QUEUE_FLUSH, null);
    }

    @Subscribe(threadMode = ThreadMode.MAIN)
    public void onEvent(Event_MyService_UDPGet event) {
        if(event.getData()[1].equals("99") || event.getData()[1].equals("100")){
            SpeechEnable = event.getData()[1];
        }
        //byte joyData[] = new byte[256];
        //joyData = event.getData();
        //txt_data.setText(String.valueOf(event.getData()[1]));

//        for(int i = 0; i > event.getData().length; i++ ){
//
//
//        }

//        textjoy0.setText(event.getData()[0]);
//        textjoy1.setText(event.getData()[1]);
//        textjoy2.setText(event.getData()[2]);
//        textjoy3.setText(event.getData()[3]);
//        textjoy4.setText(event.getData()[4]);
//        textjoy5.setText(event.getData()[5]);
//        textjoy6.setText(event.getData()[6]);
//        textjoy7.setText(event.getData()[7]);
//        textjoy8.setText(event.getData()[12]);

        if(event.getData()[12].equals(SpeechTTX)) {

        }
            else{
                if(SpeechEnable.equals("99")) {
                    ttx.speak(event.getData()[12], TextToSpeech.QUEUE_FLUSH, null);
                }


                    if (event.getData()[12].contains("три закона работа техники") || event.getData()[12].contains("три закона работы техники") || event.getData()[12].contains("три закона робототехники")
                             || event.getData()[12].contains("три закона робота техники") || event.getData()[12].contains("три закона работать техники")) {
                        ttx.speak("Робот не может причинить вред человеку или своим бездействием допустить, чтобы человеку был причинён вред.\n" +
                                        "Робот должен повиноваться всем приказам, которые даёт человек, кроме тех случаев, когда эти приказы противоречат Первому Закону.\n" +
                                        "Робот должен заботиться о своей безопасности в той мере, в которой это не противоречит Первому или Второму Законам.",
                                TextToSpeech.QUEUE_FLUSH, null);
                    }

            }
        SpeechTTX = event.getData()[12];

    }

    @Override
    protected void onDestroy() { // Закрытие приложения
        super.onDestroy();
        EventBus.getDefault().unregister(this);
    }
}

