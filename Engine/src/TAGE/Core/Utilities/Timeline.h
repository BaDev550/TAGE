#pragma once

#include <functional>
#include <vector>
#include <algorithm>

namespace TAGE {
    class Timeline {
    public:
        using Callback = std::function<void(float value)>;

        struct Animation {
            float StartValue;
            float EndValue;
            float Duration;
            float Elapsed;
            Callback OnUpdate;
            std::function<void()> OnFinished;

            bool Finished = false;

            Animation(float start, float end, float duration, Callback onUpdate, std::function<void()> onFinished = nullptr)
                : StartValue(start), EndValue(end), Duration(duration), Elapsed(0.0f), OnUpdate(onUpdate), OnFinished(onFinished) {
            }
        };

        static void Update(float deltaTime) {
            for (auto& anim : _Animations) {
                if (anim.Finished) continue;

                anim.Elapsed += deltaTime;
                float alpha = std::clamp(anim.Elapsed / anim.Duration, 0.0f, 1.0f);
                float value = Lerp(anim.StartValue, anim.EndValue, alpha);
                anim.OnUpdate(value);

                if (alpha >= 1.0f) {
                    anim.Finished = true;
                    if (anim.OnFinished) anim.OnFinished();
                }
            }

            _Animations.erase(
                std::remove_if(_Animations.begin(), _Animations.end(), [](const Animation& a) { return a.Finished; }),
                _Animations.end()
            );
        }

        static void Play(float start, float end, float duration, Callback onUpdate, std::function<void()> onFinished = nullptr) {
            _Animations.emplace_back(start, end, duration, onUpdate, onFinished);
        }

    private:
        static std::vector<Animation> _Animations;

        static float Lerp(float a, float b, float alpha) {
            return a + (b - a) * alpha;
        }
    };
}
