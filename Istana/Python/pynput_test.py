from pynput.keyboard import Key, Listener, KeyCode


def on_press(key):
    if isinstance(key, Key):
        print("key")
    elif isinstance(key, KeyCode):
        print("code")


def on_release(key):
    print('{0} release'.format(
        key))
    if key == Key.esc:
        # Stop listener
        return False


# Collect events until released
with Listener(
        on_press=on_press,
        on_release=on_release) as listener:
    listener.join()
