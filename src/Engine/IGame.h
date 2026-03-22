#pragma once

class IGame {
public:
    virtual ~IGame() = default;

    virtual void Start() = 0;

    virtual void Update() = 0;
};