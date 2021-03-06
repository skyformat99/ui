// Copyright (c) 2017 Kolya Kosenko

// Distributed under the Boost Software License, Version 1.0.
// See http://www.boost.org/LICENSE_1_0.txt

#define BOOST_UI_SOURCE

#include <boost/ui/choice.hpp>
#include <boost/ui/native/event.hpp>
#include <boost/ui/native/string.hpp>
#include <boost/ui/native/widget.hpp>
#include <boost/ui/detail/memcheck.hpp>

#include <wx/choice.h>

namespace boost {
namespace ui    {

#if wxUSE_CHOICE

class choice::detail_impl : public detail::widget_detail<wxChoice>
{
public:
    explicit detail_impl(widget& parent)
    {
        set_native_handle(new wxChoice(native::from_widget(parent), wxID_ANY));
    }

    explicit detail_impl(widget& parent, const std::vector<uistring>& options)
    {
        set_native_handle(new wxChoice(native::from_widget(parent),
            wxID_ANY, wxDefaultPosition, wxDefaultSize,
            native::from_vector_uistring(options)));
    }
    uistring text() const
    {
        wxCHECK(m_native, uistring());
        return native::to_uistring(m_native->GetStringSelection());
    }
    index_type current_selection_index() const
    {
        wxCHECK(m_native, -1);
        return m_native->GetCurrentSelection();
    }
};

#endif

choice::detail_impl* choice::get_impl()
{
#if wxUSE_CHOICE
    return get_detail_impl<detail_impl>();
#else
    return NULL;
#endif
}

const choice::detail_impl* choice::get_impl() const
{
#if wxUSE_CHOICE
    return get_detail_impl<detail_impl>();
#else
    return NULL;
#endif
}

choice& choice::create(widget& parent)
{
#if wxUSE_CHOICE
    detail_set_detail_impl(new detail_impl(parent));
#endif

    return *this;
}

choice& choice::create(widget& parent, const std::vector<uistring>& options)
{
#if wxUSE_CHOICE
    detail_set_detail_impl(new detail_impl(parent, options));
#endif

    return *this;
}

uistring choice::text() const
{
#if wxUSE_CHOICE
    const detail_impl* impl = get_impl();
    wxCHECK(impl, uistring());

    return impl->text();
#else
    return uistring();
#endif
}

index_type choice::current_selection_index() const
{
#if wxUSE_CHOICE
    const detail_impl* impl = get_impl();
    wxCHECK(impl, npos);

    return impl->current_selection_index();
#else
    return npos;
#endif
}

choice& choice::on_select(const boost::function<void()>& handler)
{
#if wxUSE_CHOICE
    native::bind_helper(*this, wxEVT_CHOICE, handler);
#endif

    return *this;
}

choice& choice::on_select_event(const boost::function<void(index_event&)>& handler)
{
#if wxUSE_CHOICE
    native::bind_event_helper(*this, wxEVT_CHOICE, handler);
#endif

    return *this;
}

} // namespace ui
} // namespace boost
