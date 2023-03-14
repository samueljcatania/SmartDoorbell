//
// Created by Jonathan Lee on 2023-03-13.
//

#include "../include/DisplayWindow.h"
#include "../include/VideoRecorder.h"

VideoRecorder cam;

void cameraButtonClick() { //Show camera when requested
    cam.openCamera();
    cam.peek();
}

void recordButtonClick() {//Start recording when asked
    cam.openCamera();
    cam.recordVideo();
}

void stopRecordButtonClick() {//Stop recording??
    cam.closeCamera();
}


DisplayWindow::DisplayWindow() {

}

DisplayWindow::~DisplayWindow() {

}


int DisplayWindow::openGui() { // function to open a window for user to control the camera
    auto app = Gtk::Application::create("org.gtkmm.examples.base");

    // Create a window with a vertical box container
    Gtk::Window window;
    window.set_border_width(10);
    window.override_background_color(Gdk::RGBA("black"));

    Gtk::Box box(Gtk::ORIENTATION_VERTICAL, 10);
    window.add(box);

    // three buttons for user to interact with
    Gtk::Button camera("Camera ON/OFF");
    Gtk::Button record("Record");
    Gtk::Button stopRecord("Stop recording");
    box.pack_start(camera);
    box.pack_start(record);
    box.pack_start(stopRecord);

    //Event handling
    camera.signal_clicked().connect(sigc::ptr_fun(&cameraButtonClick));
    record.signal_clicked().connect(sigc::ptr_fun(&recordButtonClick));
    stopRecord.signal_clicked().connect(sigc::ptr_fun(&stopRecordButtonClick));

    // Show the window and run the application
    window.show_all();
    return app->run(window);
}




