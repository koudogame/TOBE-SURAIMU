
// î¬èÍ

#pragma once

class ProgressStage
{
public:
    ProgressStage();
    ~ProgressStage();

    bool init( const Vector2& Position );
    void destroy();
    void update();
    void draw();

    const Vector2& getPosition() const { return position_; }
    void           setPosition( const Vector2& Destination ) { position_ = Destination; }

private:
    ID3D11ShaderResourceView* texture_ = nullptr;
    Vector2 position_;
};
