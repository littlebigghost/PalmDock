#ifndef VOLUME_H
#define VOLUME_H

class Volume
{
public:
    Volume();

    static bool setSysVolume(int level);
    static int sysVolume();
};

#endif // VOLUME_H
