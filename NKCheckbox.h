#pragma once
#ifndef NKCheckbox_h__
#define NKCheckbox_h__
#include "NKBase.h"
#include "NKBaseLabel.h"
#include "NKStyleCheckbox.h"
class NKCheckbox : public NKBase, public NKBaseLabel, public NKStyleCheckbox
{
public:
    NKCheckbox();
    NKCheckbox(nk_context* ctx, NuklearUI* pManager);
    NKCheckbox(const NKCheckbox& other);
    virtual ~NKCheckbox();
    

public:
    virtual void Layout(nk_context* ctx) override;
    virtual void EditInfo() override;
    virtual void EditStyle() override;

    void SetChecked(bool checked);
    bool IsChecked() const;

public:
    int m_checked;

public:
    template <class Archive>
    void serialize(Archive& ar) {
        ar(cereal::base_class<NKBase>(this)
            , cereal::base_class<NKBaseLabel>(this)
            , cereal::base_class<NKStyleCheckbox>(this)
            , m_checked
        );
    }
};
#endif //NKCheckbox_h__
