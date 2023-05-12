bool checkIfPower(int number, int power) {
    int left = 1;
    int right = number;
    int mid;
    long long tmp;
    while (left <= right) {
        mid = (right - left) / 2 + left;
        tmp = 1;
        for (int i = 0; i < power; ++i) {
            tmp = tmp * mid;
            if (tmp > number) {
                right = mid - 1;
                break;
            }
        }
        if (tmp == number) {
            return true;
        } else if (tmp < number) {
            left = mid + 1;
        }
    }
    return false;
}