package org.cocos2dx.lib;

import java.lang.reflect.Method;
import java.net.URI;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.provider.MediaStore;
import android.util.Log;
import android.view.Gravity;
import android.webkit.ValueCallback;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.FrameLayout;

public class Cocos2dxWebView extends WebView {
    private static final String TAG = Cocos2dxWebViewHelper.class.getSimpleName();

    private int mViewTag;
    private String mJSScheme;
    ValueCallback<Uri> mUploadMessage;

    public Cocos2dxWebView(Context context) {
        this(context, -1);
    }

    @SuppressLint("SetJavaScriptEnabled")
    public Cocos2dxWebView(Context context, int viewTag) {
        super(context);
        this.mViewTag = viewTag;
        this.mJSScheme = "";

        this.setFocusable(true);
        this.setFocusableInTouchMode(true);

        this.getSettings().setSupportZoom(false);

        this.getSettings().setJavaScriptEnabled(true);
        
        this.getSettings().setCacheMode(WebSettings.LOAD_NO_CACHE);

        // `searchBoxJavaBridge_` has big security risk. http://jvn.jp/en/jp/JVN53768697
        try {
            Method method = this.getClass().getMethod("removeJavascriptInterface", new Class[]{String.class});
            method.invoke(this, "searchBoxJavaBridge_");
        } catch (Exception e) {
            Log.d(TAG, "This API level do not support `removeJavascriptInterface`");
        }

        this.setWebViewClient(new Cocos2dxWebViewClient());
        this.setWebChromeClient(new MyWebChromeClient());
    }

    public void setJavascriptInterfaceScheme(String scheme) {
        this.mJSScheme = scheme != null ? scheme : "";
    }

    public void setScalesPageToFit(boolean scalesPageToFit) {
        this.getSettings().setSupportZoom(scalesPageToFit);
    }

    class Cocos2dxWebViewClient extends WebViewClient {
        @Override
        public boolean shouldOverrideUrlLoading(WebView view, String urlString) {
            URI uri = URI.create(urlString);
            if (uri != null && uri.getScheme().equals(mJSScheme)) {
                Cocos2dxWebViewHelper._onJsCallback(mViewTag, urlString);
                return true;
            }
            return Cocos2dxWebViewHelper._shouldStartLoading(mViewTag, urlString);
        }

        @Override
        public void onPageFinished(WebView view, String url) {
            super.onPageFinished(view, url);
            Cocos2dxWebViewHelper._didFinishLoading(mViewTag, url);
        }

        @Override
        public void onReceivedError(WebView view, int errorCode, String description, String failingUrl) {
            super.onReceivedError(view, errorCode, description, failingUrl);
            Cocos2dxWebViewHelper._didFailLoading(mViewTag, failingUrl);
        }
    }
    
    public void setWebViewRect(int left, int top, int maxWidth, int maxHeight) {
        FrameLayout.LayoutParams layoutParams = new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT,
                FrameLayout.LayoutParams.WRAP_CONTENT);
        layoutParams.leftMargin = left;
        layoutParams.topMargin = top;
        layoutParams.width = maxWidth;
        layoutParams.height = maxHeight;
        layoutParams.gravity = Gravity.TOP | Gravity.LEFT;
        this.setLayoutParams(layoutParams);
    }

    public static final int FILECHOOSER_RESULTCODE = 1;
    private static final int REQ_CHOOSE = FILECHOOSER_RESULTCODE+1;
    
    private class MyWebChromeClient extends WebChromeClient {
        // For Android 3.0+
        public void openFileChooser(ValueCallback<Uri> uploadMsg, String acceptType) {
            if (mUploadMessage != null) return;
            mUploadMessage = uploadMsg;
            Intent innerIntent = new Intent(Intent.ACTION_GET_CONTENT); // "android.intent.action.GET_CONTENT"
            String IMAGE_UNSPECIFIED = "image/*";
            innerIntent.setType(IMAGE_UNSPECIFIED); // 查看类型
            Intent wrapperIntent = Intent.createChooser(innerIntent, null);
            Cocos2dxActivity act = (Cocos2dxActivity) Cocos2dxActivity.getContext();
            act.startActivityForResult(wrapperIntent, REQ_CHOOSE);
        }
        // For Android < 3.0
        public void openFileChooser(ValueCallback<Uri> uploadMsg) {
            openFileChooser(uploadMsg, "");
        }
        // For Android  > 4.1.1
        public void openFileChooser(ValueCallback<Uri> uploadMsg, String acceptType, String capture) {
            openFileChooser(uploadMsg, acceptType);
        }
    }
    
    public boolean onActivityResult(int requestCode, int resultCode, Intent intent) {
        if (null == mUploadMessage)
            return false;
        Uri uri = null;
        if(requestCode == REQ_CHOOSE && intent != null && intent.getData() != null){
            String[] proj = { MediaStore.Images.Media.DATA };
            Cocos2dxActivity act = (Cocos2dxActivity) Cocos2dxActivity.getContext();
            Cursor cursor = act.getContentResolver().query(intent.getData(), proj, null, null, null);
            if(cursor != null ){
                int column_index = cursor.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);
                cursor.moveToFirst();
                uri = Uri.parse(cursor.getString(column_index));
                cursor.close();
            }
        }
        mUploadMessage.onReceiveValue(uri);
        mUploadMessage = null;
        return true;
    };
}
