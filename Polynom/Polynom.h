#pragma once

#include <vector>

template<typename T>
class Polynom {
public:
    Polynom() = default;

    Polynom(int a) {
        data_.resize(a+1, 0);
    }

    Polynom(const Polynom<T>& other) {
        data_ = other.data_;
    }

    Polynom<T>& operator=(const Polynom<T>& other) = default;

    Polynom<T>& operator*=(const Polynom<T>& other) {
        std::vector<T> new_data(this->data_.size() + other.data_.size(), 0);
        for (int i = 0; i < data_.size(); i++) {
            for (int j = 0; j < other.data_.size(); j++) {
                new_data[i + j] += data_[i] * other.data_[j];
            }
        }
        for (int & i : new_data) {
            i %= 2;
        }
        data_ = new_data;
        return (*this);
    }

    Polynom<T>& operator+=(const Polynom<T>& other) {
        std::vector<T> new_data(std::max(data_.size(), other.data_.size()), 0);
        for (int i = 0; i < data_.size(); i++) {
            new_data[i] += data_[i];
        }
        for (int i = 0; i < other.data_.size(); i++) {
            new_data[i] += other.data_[i];
        }
        for (int & i : new_data) {
            i %= 2;
        }
        data_ = new_data;
        return (*this);
    }

    Polynom<T>& operator-=(const Polynom<T>& other) {
        std::vector<T> new_data(std::max(data_.size(), other.data_.size()), 0);
        for (int i = 0; i < data_.size(); i++) {
            new_data[i] = (new_data[i] - data_[i] + 2) % 2;
        }
        for (int i = 0; i < other.data_.size(); i++) {
            new_data[i] = (new_data[i] - other.data_[i] + 2) % 2;
        }
        data_ = new_data;
        return (*this);
    }

    Polynom<T>& operator%=(const Polynom<T>& other) {
        while (maxStep() >= other.maxStep()) {
            Polynom<T> a;
            a.data_.resize(maxStep() - other.maxStep() + 1, 0);
            a.data_[maxStep() - other.maxStep()] = 1;
            Polynom<T> prom = a * other;
            (*this) -= prom;
        }
        return (*this);
    }

    T const &operator[](int ind) const {
        return data_[ind];
    }
    T& operator[](int ind) {
        return data_[ind];
    }

    template <T> friend Polynom<T> operator*(const Polynom<T>& a, const Polynom<T> &b);
    template <T> friend Polynom<T> operator+(const Polynom<T>& a, const Polynom<T>& b);
    template <T> friend Polynom<T> operator-(const Polynom<T>& a, const Polynom<T>& b);
    template <T> friend Polynom<T> operator%(const Polynom<T>& a, const Polynom<T>& b);

    static Polynom<T> getPol(int s) {
        Polynom<T> ans;
        ans.data_.resize(s+1, 0);
        ans.data_[s] = 1;
        return ans;
    }

    int maxStep() const {
        for (int i = (int)data_.size() - 1; i >= 0; i--) {
            if (data_[i] != 0) {
                return i;
            }
        }
        return -1;
    }

    void print() const {
        for (int i = 0; i <= maxStep(); i++) {
            std::cout << data_[i];
        }
        std::cout << "\n";
    }

private:
    std::vector<T> data_;
};

template<typename T>
Polynom<T> operator*(const Polynom<T>& a, const Polynom<T>& b) {
    Polynom ans(a);
    ans *= b;
    return ans;
}

template<typename T>
Polynom<T> operator+(const Polynom<T>& a, const Polynom<T>& b) {
    Polynom ans(a);
    ans += b;
    return ans;
}

template<typename T>
Polynom<T> operator-(const Polynom<T>& a, const Polynom<T>& b) {
    Polynom ans(a);
    ans -= b;
    return ans;
}

template<typename T>
Polynom<T> operator%(const Polynom<T>& a, const Polynom<T>& b) {
    Polynom ans(a);
    ans %= b;
    return ans;
}