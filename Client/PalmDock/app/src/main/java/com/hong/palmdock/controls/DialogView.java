package com.hong.palmdock.controls;

import android.app.Dialog;
import android.content.Context;
import android.view.Window;
import android.view.WindowManager;

import androidx.annotation.NonNull;

import com.hong.palmdock.R;

public class DialogView extends Dialog {
    public DialogView(@NonNull Context context, int layout, int style, int gravity) {
        super(context);
        setContentView(layout);
        Window mWindow = getWindow();
        WindowManager.LayoutParams params = mWindow.getAttributes();
        mWindow.setBackgroundDrawableResource(R.drawable.shape_server_item);
        params.width = WindowManager.LayoutParams.MATCH_PARENT;
        params.height = WindowManager.LayoutParams.WRAP_CONTENT;
        params.gravity = gravity;
        mWindow.setAttributes(params);
    }
}
