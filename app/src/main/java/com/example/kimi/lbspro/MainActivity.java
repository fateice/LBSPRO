package com.example.kimi.lbspro;

import android.annotation.SuppressLint;
import android.content.ContentResolver;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.location.Criteria;
import android.location.Location;
import android.location.LocationManager;
import android.os.Bundle;
import android.os.SystemClock;
import android.provider.Settings;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

import com.baidu.location.BDLocation;
import com.baidu.location.BDLocationListener;
import com.baidu.location.LocationClient;
import com.baidu.location.LocationClientOption;
import com.baidu.mapapi.SDKInitializer;
import com.baidu.mapapi.map.BaiduMap;
import com.baidu.mapapi.map.BitmapDescriptor;
import com.baidu.mapapi.map.BitmapDescriptorFactory;
import com.baidu.mapapi.map.MapStatus;
import com.baidu.mapapi.map.MapStatusUpdate;
import com.baidu.mapapi.map.MapStatusUpdateFactory;
import com.baidu.mapapi.map.MapView;
import com.baidu.mapapi.map.MarkerOptions;
import com.baidu.mapapi.map.MyLocationConfiguration;
import com.baidu.mapapi.map.MyLocationData;
import com.baidu.mapapi.map.OverlayOptions;
import com.baidu.mapapi.map.PolygonOptions;
import com.baidu.mapapi.map.Stroke;
import com.baidu.mapapi.model.LatLng;

import java.util.ArrayList;
import java.util.List;


public class MainActivity extends AppCompatActivity {
    private static final String TAG = "SetLocation";
    private Thread thread;
    private MapView mMapView=null;
    private BaiduMap mBaiduMap;
    private LocationClient mlocationClient;
    private MylocationListener mlistener;
    private Context context;

    private LocationManager mLocationManager;
    private String mMockProviderName = LocationManager.GPS_PROVIDER;

    //实际经纬度
    private double mLatitude;
    private double mLongitude;

    //模拟经纬度
    private double mmLatitude = 40.052462;
    private double mmLongitude = 116.29064;
    private float mCurrentX;

    //计算后的经纬度
    private double ss[];
    private int k = 3;
    private int s = 10000;

    private Button mButton;

//    private EditText editTextk = (EditText)findViewById(R.id.editk);
//    private EditText editTexts = (EditText)findViewById(R.id.edits);

    private EditText editTextk;
    private EditText editTexts;

    private  TextView mStateBar;


    boolean siming = false;

    /**
     * 当前地点击点
     */
    private LatLng currentPt;
    private String touchType;

    private ImageButton mGetMylocationBN;

    //自定义图标
    private BitmapDescriptor mIconLocation;

    private MyOrientationListener myOrientationListener;
    //定位图层显示方式
    private MyLocationConfiguration.LocationMode locationMode;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        // requestWindowFeature(Window.FEATURE_NO_TITLE);
        SDKInitializer.initialize(getApplicationContext());
        setContentView(R.layout.activity_main);
        this.context=this;

        mStateBar = (TextView)findViewById(R.id.mStateBar);
        mButton = (Button) findViewById(R.id.button);

        mLocationManager = (LocationManager) context.getSystemService(Context.LOCATION_SERVICE);
        enableTestProvider();
        //setLocation();

//        thread = new Thread(new Runnable() {
//            @Override
//            public void run() {
//                try{
//                    Thread.sleep(500);
//                }catch(InterruptedException e){
//                    e.printStackTrace();
//                }
//                setLocation();
//            }
//        });

        initView();
        initListener();
        initLocation();


    }

    public native double[] GridDummy(double mLongitude,double mLatitude,int k,double s);//栅格
    public native double[] NNC(double mLongitude,double mLatitude, int k, double s);//最近邻
    public native double[] IC(double mLongitude,double mLatitude,int k,double s);//四叉树
    public native double[] Casper(double mLongitude,double mLatitude,int k,double s);//四叉树改进

    public void inputks(View view)
    {
        editTextk = (EditText)findViewById(R.id.editk);
        editTexts = (EditText)findViewById(R.id.edits);
        k = Integer.parseInt(editTextk.getText().toString());
        s = Integer.parseInt(editTexts.getText().toString());
    }

    public void CCasper(View view)
    {
        mBaiduMap.clear();
        ss = Casper(mLongitude,mLatitude,k,s);
        LatLng pt1 = new LatLng(ss[1],ss[0]);
        LatLng pt2 = new LatLng(ss[3],ss[0]);
        LatLng pt3 = new LatLng(ss[3],ss[2]);
        LatLng pt4 = new LatLng(ss[1],ss[2]);
        List<LatLng> pts = new ArrayList<LatLng>();
        pts.add(pt1);
        pts.add(pt2);
        pts.add(pt3);
        pts.add(pt4);
        OverlayOptions poly = new PolygonOptions().points(pts).stroke(new Stroke(4,0xAA0066CC)).fillColor(0xAAFFCCFF);
        mBaiduMap.addOverlay(poly);

        BitmapDescriptor bitmap = BitmapDescriptorFactory.fromResource(R.drawable.location_m);
        for(int i=0;i<10;i++)
        {
            LatLng point = new LatLng(ss[i+14],ss[i+4]);
            OverlayOptions option = new MarkerOptions().position(point).icon(bitmap);
            mBaiduMap.addOverlay(option);
        }
    }

    public void ICb(View view)
    {

        mBaiduMap.clear();
        //ss = IC(mLongitude,mLatitude,3,10000);
        ss = IC(mLongitude,mLatitude,k,s);
        LatLng pt1 = new LatLng(ss[1],ss[0]);
        LatLng pt2 = new LatLng(ss[3],ss[0]);
        LatLng pt3 = new LatLng(ss[3],ss[2]);
        LatLng pt4 = new LatLng(ss[1],ss[2]);
        List<LatLng> pts = new ArrayList<LatLng>();
        pts.add(pt1);
        pts.add(pt2);
        pts.add(pt3);
        pts.add(pt4);
        OverlayOptions poly = new PolygonOptions().points(pts).stroke(new Stroke(4,0xAA0066CC)).fillColor(0xAAFFCCFF);
        mBaiduMap.addOverlay(poly);

        BitmapDescriptor bitmap = BitmapDescriptorFactory.fromResource(R.drawable.location_m);
        for(int i=0;i<10;i++)
        {
            LatLng point = new LatLng(ss[i+14],ss[i+4]);
            OverlayOptions option = new MarkerOptions().position(point).icon(bitmap);
            mBaiduMap.addOverlay(option);
        }
    }

    public void NNCb(View view)
    {
        mBaiduMap.clear();
        ss = NNC(mLongitude,mLatitude,k,s);
        LatLng pt1 = new LatLng(ss[1],ss[0]);
        LatLng pt2 = new LatLng(ss[3],ss[0]);
        LatLng pt3 = new LatLng(ss[3],ss[2]);
        LatLng pt4 = new LatLng(ss[1],ss[2]);
        List<LatLng> pts = new ArrayList<LatLng>();
        pts.add(pt1);
        pts.add(pt2);
        pts.add(pt3);
        pts.add(pt4);
        OverlayOptions poly = new PolygonOptions().points(pts).stroke(new Stroke(4,0xAA0066CC)).fillColor(0xAAFFCCFF);
        mBaiduMap.addOverlay(poly);

        BitmapDescriptor bitmapk = BitmapDescriptorFactory.fromResource(R.drawable.location_kmean);
        LatLng pointkmean = new LatLng(ss[25],ss[24]);
        OverlayOptions optionkmean = new MarkerOptions().position(pointkmean).icon(bitmapk);
        mBaiduMap.addOverlay(optionkmean);

        BitmapDescriptor bitmap = BitmapDescriptorFactory.fromResource(R.drawable.location_m);
        for(int i=0;i<10;i++)
        {
            LatLng point = new LatLng(ss[i+14],ss[i+4]);
            OverlayOptions option = new MarkerOptions().position(point).icon(bitmap);
            mBaiduMap.addOverlay(option);
        }

    }

    public void Grid(View view)
    {
        ss=GridDummy(mLongitude,mLatitude,25,0.01);
        int RealPos =(new Double(ss[200])).intValue();

        mBaiduMap.clear();
        BitmapDescriptor bitmap = BitmapDescriptorFactory.fromResource(R.drawable.location_m);
        for (int i=0;i<25;i++)
        {
            LatLng point = new LatLng(ss[i+100],ss[i]);
            OverlayOptions option = new MarkerOptions().position(point).icon(bitmap);
            mBaiduMap.addOverlay(option);
        }




//        OverlayOptions Dot = new DotOptions().center(point).color(0xAAFFFF00).radius(2);
//        mBaiduMap.addOverlay(Dot);
    }

    public void simGPS(View view){
        //thread.start();
        setLocation();
    }

    public void simClose(View view){
        //mLocationManager.clearTestProviderEnabled(mMockProviderName);
        //mLocationManager.removeTestProvider(mMockProviderName);
        //initView();
        //initListener();
        //initLocation();
        mBaiduMap.clear();
    }

    private void initView() {
        mMapView= (MapView) findViewById(R.id.bmapView);

        mBaiduMap=mMapView.getMap();
        mBaiduMap.setMapType(BaiduMap.MAP_TYPE_NORMAL);

        //根据给定增量缩放地图级别
        MapStatusUpdate msu= MapStatusUpdateFactory.zoomTo(18.0f);
        mBaiduMap.setMapStatus(msu);
        mGetMylocationBN=(ImageButton)findViewById(R.id.id_bn_getMyLocation);
        mGetMylocationBN.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                getMyLocation();
            }
        });

    }

    private void initListener(){
        mBaiduMap.setOnMapTouchListener(new BaiduMap.OnMapTouchListener() {
            @Override
            public void onTouch(MotionEvent motionEvent) {

            }
        });


        mBaiduMap.setOnMapLongClickListener( new BaiduMap.OnMapLongClickListener() {
            @Override
            public void onMapLongClick(LatLng point) {
                touchType = "长按";
                currentPt = point;
                updateMapState();
            }
        });

        mBaiduMap.setOnMapStatusChangeListener(new BaiduMap.OnMapStatusChangeListener() {
            @Override
            public void onMapStatusChangeStart(MapStatus mapStatus) {
                updateMapState();
            }

            @Override
            public void onMapStatusChange(MapStatus mapStatus) {
                // updateMapState();
            }

            @Override
            public void onMapStatusChangeFinish(MapStatus mapStatus) {
                updateMapState();
            }
        });
    }

    private void updateMapState(){
        if(mStateBar == null) {
            return;
        }
        String state = "";
        if (currentPt == null) {
            state = "";
        }else
        {
            state = String.format("当前经度： %f 当前纬度：%f",currentPt.longitude,currentPt.latitude);
        }
        state += "\n";
        mStateBar.setText(state);
    }

    private void initLocation() {
        locationMode= MyLocationConfiguration.LocationMode.NORMAL;

        //定位服务的客户端。宿主程序在客户端声明此类，并调用，目前只支持在主线程中启动
        mlocationClient=new LocationClient(this);
        mlistener=new MylocationListener();

        //注册监听器
        mlocationClient.registerLocationListener(mlistener);
        //配置定位SDK各配置参数，比如定位模式、定位时间间隔、坐标系类型等
        LocationClientOption mOption=new LocationClientOption();
        //设置坐标类型
        mOption.setCoorType("bd09ll");
        //设置是否需要地址信息，默认为无地址
        mOption.setIsNeedAddress(true);
        //设置是否打开gps进行定位
        mOption.setOpenGps(true);
        //设置扫描间隔，单位是毫秒 当<1000(1s)时，定时定位无效
        int span=1000;
        mOption.setScanSpan(span);

        mOption.setEnableSimulateGps(true);
        //设置 LocationClientOption
        mlocationClient.setLocOption(mOption);

//        //初始化图标,BitmapDescriptorFactory是bitmap 描述信息工厂类.
//        mIconLocation= BitmapDescriptorFactory
//                .fromResource(R.drawable.gez_location_marker2);

        Bitmap bm = BitmapFactory.decodeResource(getResources(),R.drawable.gez_location_marker);
        int width = bm.getWidth();
        int height = bm.getHeight();
        float radio = (float)0.4;
        Matrix matrix = new Matrix();
        matrix.postScale(radio, radio);
        // 得到新的图片
        Bitmap newbm = Bitmap.createBitmap(bm, 0, 0, width, height, matrix, true);
        mIconLocation = BitmapDescriptorFactory.fromBitmap(newbm);



        myOrientationListener=new MyOrientationListener(context);
        //通过接口回调来实现实时方向的改变
        myOrientationListener.setOnOrientationListener(new MyOrientationListener.OnOrientationListener() {
            @Override
            public void onOrientationChanged(float x) {
                mCurrentX=x;
            }
        });


    }




    public void enableTestProvider() {
        ContentResolver res = context.getContentResolver();
        //获取GPS状态
//        boolean gps_enable = Settings.Secure.isLocationProviderEnabled(res, LocationManager.GPS_PROVIDER);
//        if(gps_enable){
//            Settings.Secure.setLocationProviderEnabled(res,LocationManager.GPS_PROVIDER,false);
//        }
//        //获取允许位置模拟
//        int mock_enable = Settings.Secure.getInt(res,Settings.Secure.ALLOW_MOCK_LOCATION,0);
//        if(mock_enable == 0)
//        {
//            try{
//                Settings.Secure.putInt(res,Settings.Secure.ALLOW_MOCK_LOCATION,1);
//            }catch (Exception e) {
//                Log.e(TAG,"write error",e);
//            }
//        }
        mLocationManager.addTestProvider(mMockProviderName,
                "requiresNetwork" == "", "requiresSatellite" == "",
                "requiresCell" == "", "hasMonetaryCost" == "",
                "supportsAltitude" == "", "supportsSpeed" == "",
                "supportsBearing" == "supportsBearing",
                Criteria.POWER_LOW,
                Criteria.ACCURACY_FINE
        );
        mLocationManager.setTestProviderEnabled(mMockProviderName,true);

    }

    @SuppressLint("NewApi")
    private void setLocation(){
        Log.d(TAG,"setLocation");
        Location loc = new Location(mMockProviderName);
        loc.setAccuracy(Criteria.ACCURACY_FINE);
        loc.setTime(System.currentTimeMillis());
        mmLatitude = currentPt.latitude;
        mmLongitude = currentPt.longitude;
        loc.setLatitude(mmLatitude);
        loc.setLongitude(mmLongitude);
        loc.setElapsedRealtimeNanos(SystemClock.elapsedRealtimeNanos());
        mLocationManager.setTestProviderLocation(mMockProviderName,loc);
    }

    public void unenableTestProvider(){
        int mock_enable = Settings.Secure.getInt(
                context.getContentResolver(), Settings.Secure.ALLOW_MOCK_LOCATION, 0);
        if(mock_enable == 0) return;
        try {
            mLocationManager.clearTestProviderEnabled(mMockProviderName);
            mLocationManager.removeTestProvider(mMockProviderName);
        } catch (Exception e) {
            Log.e(TAG, "", e);
        }
//        try {
//            //关闭 允许模拟地点
//            Settings.Secure.putInt(context.getContentResolver(), Settings.Secure.ALLOW_MOCK_LOCATION, 0);
//        } catch (Exception e) {
//            Log.e(TAG, "write error", e);
//        }
    }

    @Override
    protected void onStart() {
        super.onStart();
        //开启定位
        mBaiduMap.setMyLocationEnabled(true);
        if(!mlocationClient.isStarted())
        {
            mlocationClient.start();
        }
        myOrientationListener.start();
    }

    @Override
    protected void onStop() {
        super.onStop();
        //停止定位
        mBaiduMap.setMyLocationEnabled(false);
        mlocationClient.stop();
        myOrientationListener.stop();
    }

    @Override
    protected void onResume() {
        super.onResume();
        mMapView.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        mMapView.onPause();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        unenableTestProvider();
        mMapView.onDestroy();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {

        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    public void getMyLocation()
    {
        LatLng latLng=new LatLng(mLatitude,mLongitude);
        MapStatusUpdate msu= MapStatusUpdateFactory.newLatLng(latLng);

        if(mStateBar == null) {
            return;
        }
        String state = "";
        state = String.format("当前经度： %f 当前纬度：%f",mLongitude,mLatitude);
        state += "\n";
        mStateBar.setText(state);

        mBaiduMap.setMapStatus(msu);
    }


    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId())
        {
            case R.id.id_map_common:
                mBaiduMap.setMapType(BaiduMap.MAP_TYPE_NORMAL);
                break;
            case R.id.id_map_site:
                mBaiduMap.setMapType(BaiduMap.MAP_TYPE_SATELLITE);
                break;
            case R.id.id_map_traffic:
                if(mBaiduMap.isTrafficEnabled())
                {
                    mBaiduMap.setTrafficEnabled(false);
                    item.setTitle("实时交通(off)");
                }else
                {
                    mBaiduMap.setTrafficEnabled(true);
                    item.setTitle("实时交通(on)");
                }
                break;
            case R.id.id_map_mlocation:
                getMyLocation();
                break;
            case R.id.id_map_model_common:
                //普通模式
                locationMode= MyLocationConfiguration.LocationMode.NORMAL;
                break;
            case R.id.id_map_model_following:
                //跟随模式
                locationMode= MyLocationConfiguration.LocationMode.FOLLOWING;
                break;
            case R.id.id_map_model_compass:
                //罗盘模式
                locationMode= MyLocationConfiguration.LocationMode.COMPASS;
                break;
            case R.id.id_information:
                Intent intent = new Intent(MainActivity.this,InformationActivity.class);
                startActivity(intent);
        }

        return super.onOptionsItemSelected(item);
    }
    //所有的定位信息都通过接口回调来实现
    public class MylocationListener implements BDLocationListener
    {
        //定位请求回调接口
        private boolean isFirstIn=true;
        //定位请求回调函数,这里面会得到定位信息
        @Override
        public void onReceiveLocation(BDLocation bdLocation) {
            //BDLocation 回调的百度坐标类，内部封装了如经纬度、半径等属性信息
            //MyLocationData 定位数据,定位数据建造器
            /*
            * 可以通过BDLocation配置如下参数
            * 1.accuracy 定位精度
            * 2.latitude 百度纬度坐标
            * 3.longitude 百度经度坐标
            * 4.satellitesNum GPS定位时卫星数目 getSatelliteNumber() gps定位结果时，获取gps锁定用的卫星数
            * 5.speed GPS定位时速度 getSpeed()获取速度，仅gps定位结果时有速度信息，单位公里/小时，默认值0.0f
            * 6.direction GPS定位时方向角度
            * */
            mLatitude = bdLocation.getLatitude();
            mLongitude=bdLocation.getLongitude();

            MyLocationData data= new MyLocationData.Builder()
                    .direction(mCurrentX)//设定图标方向
                    .accuracy(bdLocation.getRadius())//getRadius 获取定位精度,默认值0.0f
                    .latitude(mLatitude)//百度纬度坐标
                    .longitude(mLongitude)//百度经度坐标
                    .build();
            //设置定位数据, 只有先允许定位图层后设置数据才会生效，参见 setMyLocationEnabled(boolean)
            mBaiduMap.setMyLocationData(data);
            //配置定位图层显示方式,三个参数的构造器
            /*
            * 1.定位图层显示模式
            * 2.是否允许显示方向信息
            * 3.用户自定义定位图标
            *
            * */
            MyLocationConfiguration configuration
                    =new MyLocationConfiguration(locationMode,true,mIconLocation);
            //设置定位图层配置信息，只有先允许定位图层后设置定位图层配置信息才会生效，参见 setMyLocationEnabled(boolean)
            mBaiduMap.setMyLocationConfigeration(configuration);
            //判断是否为第一次定位,是的话需要定位到用户当前位置
            if(isFirstIn)
            {
                //地理坐标基本数据结构
                LatLng latLng=new LatLng(bdLocation.getLatitude(),bdLocation.getLongitude());
                //描述地图状态将要发生的变化,通过当前经纬度来使地图显示到该位置
                MapStatusUpdate msu= MapStatusUpdateFactory.newLatLng(latLng);
                //改变地图状态
                mBaiduMap.setMapStatus(msu);
                isFirstIn=false;
                Toast.makeText(context, bdLocation.getAddrStr(), Toast.LENGTH_SHORT).show();
            }


        }

        @Override
        public void onConnectHotSpotMessage(String s, int i) {

        }
    }

}

