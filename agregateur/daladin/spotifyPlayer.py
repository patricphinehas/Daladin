import threading
import spotify


class SpotifyPlayer(object):
    """docstring for SpotifyPlayer"""
    def connection_state_listener(session):
            if self.session.connection.state is spotify.ConnectionState.LOGGED_IN:
                logged_in_event.set()

    def __init__(self, arg):
        super(SpotifyPlayer, self).__init__()
        self.spotifyURI = arg
        self.config = spotify.Config()
        self.config.user_agent = 'Daladin Radio '
        self.config.tracefile = b'/tmp/libspotify-trace.log'
        self.config.load_application_key_file('daladin_spotify.key')
        self.session = spotify.Session(self.config)
        loop = spotify.EventLoop(self.session)
        loop.start()
        self.session.on(
            spotify.SessionEvent.CONNECTION_STATE_UPDATED,
            self.connection_state_listener)
        audio = spotify.AlsaSink(self.session)
        self.session.login('alice', 's3cr3tp4ssw0rd')
        logged_in_event = threading.Event()

    def preload(self):
        pass

    def play(self):
        logged_in_event.wait()
        for track in self.listing.tracks:
            track.load()
            self.session.player.load(track)
            self.session.player.play()
