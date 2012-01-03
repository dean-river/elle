# -*- encoding: utf-8 -*-

import os
import json
import httplib2
import urllib2
import urllib
import poster
import web

class ApiClient(object):

    _status = {
        403: web.Forbidden,
        404: web.NotFound,
    }

    def __init__(self, server, session):
        self._session = session
        self._server = server.rstrip('/') + '/'

    def get(self, url, params={}, token=None):
        params['token'] = token or self._session.get('token')
        params['fmt'] = 'json'
        url = self._server + url.lstrip('/') + '?' + urllib.urlencode(params)
        client = httplib2.Http()
        return self._getContent(*client.request(url, 'GET'))


    def post(self, url, params={}, token=None):
        params['token'] = token or self._session.get('token')
        params['fmt'] = 'json'
        url = self._server + url.lstrip('/')
        body = urllib.urlencode(params)
        client = httplib2.Http()
        headers = {'Content-Type': 'application/x-www-form-urlencoded'}
        return self._getContent(*client.request(url, 'POST', body=body, headers=headers))

    def _getContent(self, response, content):
        if response.status != 200:
            exception = self._status.get(response.status)
            if exception:
                raise exception()
            else:
                raise Exception(content)
        if content:
            return json.loads(content)
        else:
            return None

    def post_multipart(self, url, params, token=None):
        opener = poster.streaminghttp.register_openers()
        params['token'] = token or self._session.get('token')
        fields = []
        files = []
        for k, v in params.iteritems():
            if isinstance(v, tuple):
                f, fname = v
                if hasattr(f, 'read'):
                    if hasattr(f, 'fileno'):
                        files.append((k, f, fname))
                    elif isinstance(v, tuple):
                        s = v[0].read()
                        files.append((k, s, v[1]))
                    else:
                        fields.append((k, v.read()))
                else:
                    raise Exception("tuple has to contain a file-like object, and the filename")
            else:
                fields.append((k, v))
        headers, gen = self._genPostData(fields, files)
        request = urllib2.Request(url, gen, headers)
        content = urllib2.urlopen(request)
        if content:
            return json.loads(content.read())
        else:
            return None

    def _genPostData(self, fields, files):
        boundary = "THis_is_A_BOUNDARY_________________<<<<:"
        CRLF = '\r\n'
        start = ''
        for k, v in fields:
            start += CRLF.join([
                '--' + boundary,
                'Content-Disposition: form-data; name="%s"' % k,
                '',
                str(v),
                '',
            ])
        fsize = 0
        to_send = []
        for k, f, fname in files:
            if hasattr(f, 'fileno'):
                size = os.fstat(f.fileno()).st_size
            else:
                size = len(f)
            h = CRLF.join([
                '--' + boundary,
                'Content-Disposition: form-data; name="%s"; filename="%s"' % (k, fname),
                'Content-Type: octet/stream',
                '',
                '',
            ])
            to_send.append((k, f, fname, size, h))
            fsize += size + len(h) + 2

        end = CRLF.join([
            '--' + boundary + '--',
            '',
        ])

        class Yielder(object):
            def __init__(self, start, to_send, end):
                self.start = start
                self.to_send = to_send
                self.end = end
            def __iter__(self):
                self.g = self.gen()
                return self
            def next(self):
                res = self.g.next()
                if res is None:
                    raise StopIteration()
                return res
            def gen(self):
                self._sent = 0
                yield self.start
                for k, f, fname, size, headers in self.to_send:
                    yield headers
                    if isinstance(f, basestring):
                        yield f
                    else:
                        while True:
                            data = f.read(4096)
                            if not data:
                                break
                            yield data
                    yield CRLF
                yield self.end

        form_headers = {
            'Content-Type': "multipart/form-data; boundary=%s" % boundary,
            'Content-Length': str(len(start) + fsize + len(end)),
        }
        return form_headers, Yielder(start, to_send, end)

