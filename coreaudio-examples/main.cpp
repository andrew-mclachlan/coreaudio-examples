#include <AudioToolbox/AudioToolbox.h>
#include <CoreFoundation/CoreFoundation.h>

// Define a callback to be called when the sound is finished
// playing. Useful when you need to free memory after playing.

static void MyCompletionCallback(
    SystemSoundID mySSID,
    void *myURLRef)
{
    AudioServicesDisposeSystemSoundID(mySSID);
    CFRelease(myURLRef);
    CFRunLoopStop(CFRunLoopGetCurrent());
}

int main(int argc, const char *argv[])
{
    // Set up the pieces needed to play a sound.
    SystemSoundID mySSID;
    CFURLRef myURLRef;
    myURLRef = CFURLCreateWithFileSystemPath(
        kCFAllocatorDefault,
        CFSTR("/Users/amclach/Work/GitHub/coreaudio-examples/woosh.aiff"),
        kCFURLPOSIXPathStyle,
        FALSE
    );

    // create a system sound ID to represent the sound file
    OSStatus error = AudioServicesCreateSystemSoundID(myURLRef, &mySSID);
    if (error != 0)
    {
        return 1;
    }

    // Register the sound completion callback.
    // Again, useful when you need to free memory after playing.
    AudioServicesAddSystemSoundCompletion(
        mySSID,
        NULL,
        NULL,
        MyCompletionCallback,
        (void *)myURLRef
    );

    // Play the sound file.
    AudioServicesPlaySystemSound(mySSID);

    // Invoke a run loop on the current thread to keep the application
    // running long enough for the sound to play; the sound completion
    // callback later stops this run loop.
    CFRunLoopRun();
    return 0;
}
