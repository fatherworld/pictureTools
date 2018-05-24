/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

//BEGIN_INCLUDE(all)
#include "Config.h"

#include <jni.h>
#include <errno.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/log.h>
#include <android_native_app_glue.h>

#include <pthread.h>
#include "string_fl.h"
#include "stdio_fl.h"
#include "memory_fl.h"
#include <stdlib.h>

#include "Format.h"

#define LOG_TAG "main"
#include "Log.h"
#include "ErrorTools.h"
#include "Dump.h"
#include "AlvaTimer.h"

#include "BmpHelper.h"

#include "ComputeH_Direct.h"
#include "TestData.h"

/**
 * Our saved state data.
 */
struct saved_state {
    float angle;
    int32_t x;
    int32_t y;
};

static int done = 0;
static pthread_t perfTestThread         = 0;

static void*
Tracker_PerfTest_Thread(void* arg) {

    int num = (int)(sizeof(points) / 4);

    float* srcPoints = (float*)calloc(1, sizeof(float) * 2 * num);
    float* dstPoints = (float*)calloc(1, sizeof(float) * 2 * num);

    srcPoints[0] = 1.0f;
    srcPoints[1] = 2.5f;

    srcPoints[2] = 1.0f + 0.8f;
    srcPoints[3] = 2.5f + 0.5f;

    srcPoints[4] = 1.3f;
    srcPoints[5] = 2.2f;

    srcPoints[6] = 1.3f + 0.33f;
    srcPoints[7] = 2.2f + 0.76f;

    srcPoints[8] = 7.35f;
    srcPoints[9] = 9.66f;

    srcPoints[10] = 7.35f + 0.22f;
    srcPoints[11] = 9.66f + 98.3f;

    srcPoints[12] = 56.33f;
    srcPoints[13] = 89.3f;

    srcPoints[14] = 56.33f + 22.4f;
    srcPoints[15] = 89.3f + 56.7f;

    srcPoints[16] = 66.4f + 55.5f;
    srcPoints[17] = 77.0f + 99.1f;

    srcPoints[18] = 66.4f;
    srcPoints[19] = 77.0f;

    memcpy_fl(dstPoints, sizeof(float) * 2 * 10, srcPoints, sizeof(float) * 2 * 10);

    float H[9], H4[9];

    pComputeH_Direct_Data mComputeHData, mComputeHData4;
    AllocComputeH_Direct_Data(&mComputeHData, 2000);
    AllocComputeH_Direct_Data(&mComputeHData4, 2000);
    setComputeH_Direct_Data(srcPoints, dstPoints, NULL, 4, mComputeHData);
    setComputeH_Direct_Data(srcPoints, dstPoints, NULL, 4, mComputeHData4);
    ComputeH_Direct(mComputeHData, H);
    ComputeH_Direct_4(mComputeHData4, H4);

    LOGE("H:\n");
    LOGE("%f %f %f\n", H[0], H[1], H[2]);
    LOGE("%f %f %f\n", H[3], H[4], H[5]);
    LOGE("%f %f %f\n", H[6], H[7], H[8]);

    LOGE("H4:\n");
    LOGE("%f %f %f\n", H4[0], H4[1], H4[2]);
    LOGE("%f %f %f\n", H4[3], H4[4], H4[5]);
    LOGE("%f %f %f\n", H4[6], H4[7], H4[8]);

    for (int i = 0; i < 4; i++) {
        srcPoints[i * 2 + 0] = (float)points[i * 4 + 0];
        srcPoints[i * 2 + 1] = (float)points[i * 4 + 1];
        dstPoints[i * 2 + 0] = (float)points[i * 4 + 2];
        dstPoints[i * 2 + 1] = (float)points[i * 4 + 3];
    }

    setComputeH_Direct_Data(srcPoints, dstPoints, NULL, 4, mComputeHData);
    setComputeH_Direct_Data(srcPoints, dstPoints, NULL, 4, mComputeHData4);

    Timer hTime;
    startTime(&hTime);
    startTime(&hTime);
    for(int i = 0; i < 10000; i ++)
        ComputeH_Direct(mComputeHData, H);
    stopTime(&hTime);
    LOGE("Time: %ld\n", diffTime(hTime));

    startTime(&hTime);
    for (int i = 0; i < 10000; i++)
        ComputeH_Direct_4(mComputeHData4, H4);
    stopTime(&hTime);
    LOGE("Time: %ld\n", diffTime(hTime));

    LOGE("H:\n");
    LOGE("%f %f %f\n", H[0], H[1], H[2]);
    LOGE("%f %f %f\n", H[3], H[4], H[5]);
    LOGE("%f %f %f\n", H[6], H[7], H[8]);

    LOGE("H4:\n");
    LOGE("%f %f %f\n", H4[0], H4[1], H4[2]);
    LOGE("%f %f %f\n", H4[3], H4[4], H4[5]);
    LOGE("%f %f %f\n", H4[6], H4[7], H4[8]);

    FreeComputeH_Direct_Data(&mComputeHData);
    free(srcPoints);
    free(dstPoints);

PERF_TEST_END:
    LOGE("fenglang thread exit\n");
    done = 1;
    pthread_exit(NULL);
}

/**
 * Shared state for our app.
 */
struct engine {
    struct android_app* app;

    int animating;
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    int32_t width;
    int32_t height;
    struct saved_state state;
};

/**
 * Initialize an EGL context for the current display.
 */
static int engine_init_display(struct engine* engine) {
    // initialize OpenGL ES and EGL

    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */
    const EGLint attribs[] = {
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_NONE
    };
    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    eglInitialize(display, 0, 0);

    /* Here, the application chooses the configuration it desires. In this
     * sample, we have a very simplified selection process, where we pick
     * the first EGLConfig that matches our criteria */
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

    surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
    context = eglCreateContext(display, config, NULL, NULL);

    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        LOGE("Unable to eglMakeCurrent");
        return -1;
    }

    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);

    engine->display = display;
    engine->context = context;
    engine->surface = surface;
    engine->width = w;
    engine->height = h;
    engine->state.angle = 0;

    // Initialize GL state.
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_DEPTH_TEST);

    return 0;
}

/**
 * Just the current frame in the display.
 */
static void engine_draw_frame(struct engine* engine) {
    if (engine->display == NULL) {
        // No display.
        return;
    }

    if(!done){
    	// Just fill the screen with a color.
    	glClearColor(((float)engine->state.x)/engine->width, engine->state.angle,
            ((float)engine->state.y)/engine->height, 1);
    }
    else{
        glClearColor((float)255.0f, 0.0f, 0.0f, 1);
    }
    glClear(GL_COLOR_BUFFER_BIT);

    eglSwapBuffers(engine->display, engine->surface);
}

/**
 * Tear down the EGL context currently associated with the display.
 */
static void engine_term_display(struct engine* engine) {
    if (engine->display != EGL_NO_DISPLAY) {
        eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (engine->context != EGL_NO_CONTEXT) {
            eglDestroyContext(engine->display, engine->context);
        }
        if (engine->surface != EGL_NO_SURFACE) {
            eglDestroySurface(engine->display, engine->surface);
        }
        eglTerminate(engine->display);
    }
    engine->animating = 0;
    engine->display = EGL_NO_DISPLAY;
    engine->context = EGL_NO_CONTEXT;
    engine->surface = EGL_NO_SURFACE;
}

/**
 * Process the next input event.
 */
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) {
    struct engine* engine = (struct engine*)app->userData;
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {

    	if(0 == perfTestThread)
    		pthread_create(&perfTestThread, NULL, Tracker_PerfTest_Thread, NULL);

        engine->animating = 1;
        engine->state.x = AMotionEvent_getX(event, 0);
        engine->state.y = AMotionEvent_getY(event, 0);
        return 1;
    }
    return 0;
}

/**
 * Process the next main command.
 */
static void engine_handle_cmd(struct android_app* app, int32_t cmd) {
    struct engine* engine = (struct engine*)app->userData;
    switch (cmd) {
        case APP_CMD_SAVE_STATE:
            // The system has asked us to save our current state.  Do so.
            engine->app->savedState = malloc(sizeof(struct saved_state));
            *((struct saved_state*)engine->app->savedState) = engine->state;
            engine->app->savedStateSize = sizeof(struct saved_state);
            break;
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            if (engine->app->window != NULL) {
                engine_init_display(engine);
                engine_draw_frame(engine);
            }
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
            engine_term_display(engine);
            break;
        case APP_CMD_GAINED_FOCUS:
            break;
        case APP_CMD_LOST_FOCUS:
            // Also stop animating.
            engine->animating = 0;
            engine_draw_frame(engine);
            break;
    }
}

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* state) {
    struct engine engine;

    // Make sure glue isn't stripped.
    app_dummy();

    memset(&engine, 0, sizeof(engine));
    state->userData = &engine;
    state->onAppCmd = engine_handle_cmd;
    state->onInputEvent = engine_handle_input;
    engine.app = state;

    if (state->savedState != NULL) {
        // We are starting with a previous saved state; restore from it.
        engine.state = *(struct saved_state*)state->savedState;
    }

    // loop waiting for stuff to do.
    while (1) {
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((ident=ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events,
                (void**)&source)) >= 0) {

            // Process this event.
            if (source != NULL) {
                source->process(state, source);
            }

            // Check if we are exiting.
            if (state->destroyRequested != 0) {
                engine_term_display(&engine);
                return;
            }
        }

        if (engine.animating) {
            // Done with events; draw next animation frame.
            engine.state.angle += .01f;
            if (engine.state.angle > 1) {
                engine.state.angle = 0;
            }

            // Drawing is throttled to the screen update rate, so there
            // is no need to do timing here.
            engine_draw_frame(&engine);
        }
    }
}
//END_INCLUDE(all)
