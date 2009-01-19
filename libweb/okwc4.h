
// -*-c++-*-
/* $Id: okwc.h 1682 2006-04-26 19:17:22Z max $ */

#ifndef _LIBWEB_OKWC4_H_
#define _LIBWEB_OKWC4_H_

#include "okcgi.h"
#include "abuf.h"
#include "aparse.h"
#include "hdr.h"
#include "str.h"
#include "web_prot.h"
#include "async.h"
#include <time.h>
#include "httpconst.h"
#include "async.h"
#include "dns.h"
#include "tame.h"
#include "okwc.h"

namespace okwc4 {

  //-----------------------------------------------------------------------

  typedef okwc3::simple_ev_t resp_ev_t;
  typedef okwc3::resp_t resp_t;
  typedef okwc3::resp_simple_t resp_simple_t;

  //-----------------------------------------------------------------------
  
  class hostargs_t {
  public:
    hostargs_t (const str &h, okws1_port_t p, bool s);
    str hostname () const { return _hostname; }
    okws1_port_t port () const { return _port; }
    bool https () const { return _https; }
    bool ssl () const { return _https; }
    str to_str () const;
  private:
    str _hostname;
    okws1_port_t _port;
    bool _https;
  };
  
  //-----------------------------------------------------------------------

  class req_t;
  
  //-----------------------------------------------------------------------

  class reqargs_t {
  public:
    reqargs_t (const str &u = NULL,
	       htpv_t v = 1, 
	       str post = NULL,
	       ptr<const cgi_t> ck = NULL,
	       ptr<vec<str> > eh = NULL,
	       ptr<const hostargs_t> ha = NULL);
    
    reqargs_t (ptr<const hostargs_t> ha, const str &u)
      : _url (u), _hostargs (ha) {}
    
    reqargs_t &set_url (const str &u);
    reqargs_t &set_post (const str &p);
    reqargs_t &set_outcookies (ptr<const cgi_t> c);
    reqargs_t &set_extra_headers (ptr<vec<str> > v);
    reqargs_t &add_header (const str &h);
    reqargs_t &set_hostargs (ptr<const hostargs_t> h);
    reqargs_t &set_content_type (const str &s);
    
    static ptr<reqargs_t> alloc (const str &url);
    
    static ptr<reqargs_t> 
    alloc_proxied (const str &url, const str &ph, okws1_port_t pp, bool s);
    
    virtual ~reqargs_t () {}

    friend class req_t;
    
  protected:
    str _url;
    
    htpv_t _version;
    
    str _post;
    str _content_type;
    ptr<const cgi_t> _outcookies;
    ptr<vec<str> > _extra_headers;
    ptr<const hostargs_t> _hostargs;
  };
  
  //-----------------------------------------------------------------------

  class req_t : public okwc3::req_t {
  public:
    req_t (ptr<const reqargs_t> ra) : _ra (ra) {}

    str get_type () const;
    const vec<str> *get_extra_headers () const;
    htpv_t get_version () const;
    str get_hdr_hostname () const;
    str get_filename () const;
    const cgi_t *get_outcookie () const;
    str get_simple_post_str () const;

    // don't support these....
    void set_post (const str &p)  {}
    void set_extra_headers (const vec<str> &v)  {}

  protected:
    ptr<const reqargs_t> _ra;
  };

  //-----------------------------------------------------------------------

  class obj_factory_t {
  public:
    obj_factory_t () {}
    virtual ~obj_factory_t () {}
    virtual ptr<resp_t> alloc_resp ();
    virtual ptr<req_t> alloc_req (ptr<const reqargs_t> ra);
  };

  //-----------------------------------------------------------------------

  class agent_get_t : public okwc3::agent_t {
  public:
    agent_get_t (ptr<hostargs_t> ha, ptr<obj_factory_t> f = NULL) 
      : okwc3::agent_t (ha->hostname (), ha->port (), ha->ssl ()),
	_hostargs (ha),
	_obj_factory (f) {}

    virtual ~agent_get_t () {}

    virtual void get (ptr<reqargs_t> ra, ptr<obj_factory_t> f, resp_ev_t ev) 
    { get_T (ra, f, ev); }

    void get (ptr<reqargs_t> ra, resp_ev_t ev) { return get (ra, NULL, ev); }

  protected:
    void get_T (ptr<reqargs_t> ra, ptr<obj_factory_t> f, resp_ev_t ev, CLOSURE);

    ptr<hostargs_t> _hostargs;
    ptr<obj_factory_t> _obj_factory;
  };

  //-----------------------------------------------------------------------

};


#endif /* _LIBWEB_OKWC4_H_ */
