int BattleTexture_EncodeWidth(int pixel_width) {
    switch (pixel_width) {
    case 8:
        return 0;
    case 16:
        return 1;
    case 32:
        return 2;
    case 64:
        return 3;
    case 128:
        return 4;
    case 256:
        return 5;
    case 512:
        return 6;
    case 1024:
        return 7;
    default:
        return 0;
    }
}

int BattleTexture_EncodeHeight(int pixel_height) {
    switch (pixel_height) {
    case 8:
        return 0;
    case 16:
        return 1;
    case 32:
        return 2;
    case 64:
        return 3;
    case 128:
        return 4;
    case 256:
        return 5;
    case 512:
        return 6;
    case 1024:
        return 7;
    default:
        return 0;
    }
}
